/**
 * @file main_tests.cc
 * @brief Unit tests for the physics model and the design patterns.
 *
 * No external dependency: a counter, two macros, and a non-zero exit
 * code if anything fails, so it drops straight into CI.
 */

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "Sky.h"
#include "MountainChain.h"
#include "PotentialField.h"
#include "AirCell.h"
#include "Drawable.h"
#include "GaussianPeak.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "constants.h"

// ------------------------------------------------------------------ harness

namespace {
int total(0);
int failures(0);

void check(bool condition, std::string const& description, int line) {
    ++total;
    if (not condition) {
        ++failures;
        std::cerr << "  FAILED (line " << line << ") : " << description << std::endl;
    }
}

void checkNear(double actual, double expected, double tolerance, std::string const& description,
               int line) {
    ++total;
    if (std::abs(actual - expected) > tolerance) {
        ++failures;
        std::cerr << "  FAILED (line " << line << ") : " << description << " ; got " << actual
                  << ", expected " << expected << std::endl;
    }
}
} // namespace

#define CHECK(cond) check((cond), #cond, __LINE__)
#define CHECK_NEAR(actual, expected, tol)                                                          \
    checkNear((actual), (expected), (tol), #actual " ~= " #expected, __LINE__)

// ------------------------------------------------------- relief : GaussianPeak

void testGaussianPeak() {
    std::cout << "GaussianPeak" << std::endl;
    GaussianPeak peak(15.0, 15.0, 15.0, 5.0, 5.0);

    // At the summit the altitude is exactly the height parameter.
    CHECK_NEAR(peak.altitude(15.0, 15.0), 15.0, 1e-12);

    // The bell curve decreases as you move away from the summit.
    CHECK(peak.altitude(15.0, 15.0) > peak.altitude(18.0, 15.0));
    CHECK(peak.altitude(18.0, 15.0) > peak.altitude(21.0, 15.0));

    // Symmetric about the summit, on both axes.
    CHECK_NEAR(peak.altitude(12.0, 15.0), peak.altitude(18.0, 15.0), 1e-12);
    CHECK_NEAR(peak.altitude(15.0, 12.0), peak.altitude(15.0, 18.0), 1e-12);

    // Far from the summit the 0.5 cutoff flattens the altitude to zero.
    CHECK_NEAR(peak.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------------- terrain: the Composite

void testMountainChain() {
    std::cout << "MountainChain (Composite)" << std::endl;

    std::vector<std::unique_ptr<Mountain>> v;
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(10.0, 10.0, 12.0, 3.0, 3.0)));
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(30.0, 30.0, 8.0, 3.0, 3.0)));
    MountainChain chain(v);

    // The chain takes the maximum of its components at every point.
    CHECK_NEAR(chain.altitude(10.0, 10.0), 12.0, 1e-12);
    CHECK_NEAR(chain.altitude(30.0, 30.0), 8.0, 1e-12);

    // The constructor moves the pointers, so the source vector is emptied.
    CHECK(chain.getChain().size() == 2);

    /* The heart of the Composite: a chain IS a mountain, so a chain can
       contain another chain and the whole behaves identically. */
    std::vector<std::unique_ptr<Mountain>> interne;
    interne.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(5.0, 5.0, 20.0, 2.0, 2.0)));

    std::vector<std::unique_ptr<Mountain>> externe;
    externe.emplace_back(std::unique_ptr<Mountain>(new MountainChain(interne)));
    externe.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(40.0, 40.0, 6.0, 2.0, 2.0)));
    MountainChain imbriquee(externe);

    CHECK_NEAR(imbriquee.altitude(5.0, 5.0), 20.0, 1e-12);
    CHECK_NEAR(imbriquee.altitude(40.0, 40.0), 6.0, 1e-12);
    CHECK_NEAR(imbriquee.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------------ thermodynamique : AirCell

void testAirCell() {
    std::cout << "AirCell (thermodynamics)" << std::endl;
    using namespace Physics;

    /* At ground level, at the free-stream wind speed, we must land exactly
       back on the conditions at infinity. This is the analytic limit. */
    AirCell sol;
    sol.setZ(0.0);
    sol.setVelocity({V_INFINITY, 0.0, 0.0});
    sol.initialize(false);

    CHECK_NEAR(sol.getTemperature(), T_INFINITY, 1e-9);
    CHECK_NEAR(sol.getPressure(), P_INFINITY, 1e-6);
    CHECK_NEAR(sol.getEnthalpy(), H_INFINITY, 1e-6);

    // At ground level the air is unsaturated, so no cloud.
    CHECK(sol.getVapourPressure() < sol.getSaturationPressure());
    CHECK(not sol.isCloudy());

    // Rising air expands and cools.
    AirCell haut;
    haut.setZ(1000.0);
    haut.setVelocity({V_INFINITY, 0.0, 0.0});
    haut.initialize(false);

    CHECK(haut.getTemperature() < sol.getTemperature());
    CHECK(haut.getPressure() < sol.getPressure());

    // High enough, vapour pressure exceeds saturation, so a cloud forms.
    CHECK(haut.getVapourPressure() > haut.getSaturationPressure());
    CHECK(haut.isCloudy());

    // Kinetic energy converts into enthalpy: at equal z, the faster the
    // wind, the colder the air.
    AirCell rapide;
    rapide.setZ(0.0);
    rapide.setVelocity({2.0 * V_INFINITY, 0.0, 0.0});
    rapide.initialize(false);
    CHECK(rapide.getTemperature() < sol.getTemperature());

    // The "below terrain" flag is preserved.
    AirCell enterre;
    enterre.initialize(true);
    CHECK(enterre.isBelowTerrain());
    CHECK(not sol.isBelowTerrain());
}

// ------------------------------------------------------ grille : Grid3D

void testSky() {
    std::cout << "Sky (3D grid)" << std::endl;

    Sky sky(20.0, 20.0, 20.0, 20.0 / 29.0, Physics::V_INFINITY);
    std::array<int, 3> const n(sky.getCellCount());

    CHECK(n[0] == 30);
    CHECK(n[1] == 30);
    CHECK(n[2] == 30);

    /* After the update every cell holds a coherent physical state, and
       isCloudy() must be callable everywhere without leaving the grid. */
    sky.update();
    int nuageux(0);
    for (int i(0); i < n[0]; ++i)
        for (int j(0); j < n[1]; ++j)
            for (int k(0); k < n[2]; ++k)
                if (sky.isCloudy(i, j, k))
                    ++nuageux;

    // With no terrain the air does not rise, so no condensation.
    CHECK(nuageux == 0);
}

// ------------------------------------------------ potential field: bounds

void testPotentialField() {
    std::cout << "PotentialField (bounds)" << std::endl;

    PotentialField field(6, 6, 6, 1.0);
    GaussianPeak relief(3.0, 3.0, 4.0, 1.0, 1.0);
    field.initialize(Physics::V_INFINITY, relief);

    CHECK(field.getCellCount()[0] == 6);
    CHECK_NEAR(field.getCellSize(), 1.0, 1e-12);

    /* Every out-of-range index must be reported as a zero potential
       without ever indexing out of bounds. Index N, not N-1, is the
       boundary case: the guard used <= and let it through. */
    CHECK(field.isPotentialZero(6, 0, 0));
    CHECK(field.isPotentialZero(0, 6, 0));
    CHECK(field.isPotentialZero(0, 0, 6));
    CHECK(field.isPotentialZero(-1, 0, 0));
    CHECK(field.isPotentialZero(0, -1, 0));
    CHECK(field.isPotentialZero(0, 0, -1));
    CHECK(field.isPotentialZero(1000, 1000, 1000));

    // A valid index is still queryable as usual.
    (void) field.isPotentialZero(5, 5, 5);
}

// --------------------------------------------- solver: the numerical core

void testSolverOnFlatTerrain() {
    std::cout << "PotentialField (solver, flat terrain)" << std::endl;

    /* A Gaussian of height 0 is a flat plain (altitude() clamps anything
       below 0.5 to zero). With no obstacle the flow must stay exactly the
       uniform free stream, which is the analytic solution of Laplace's
       equation for this geometry. Anything else means the relaxation has
       drifted away from the boundary conditions. */
    GaussianPeak flat(0.0, 0.0, 0.0, 1.0, 1.0);
    CHECK_NEAR(flat.altitude(5.0, 5.0), 0.0, 1e-12);

    PotentialField field(10, 10, 10, 1.0);
    field.initialize(Physics::V_INFINITY, flat);
    field.computeLaplacians();
    field.solve();

    for (int i(1); i < 9; ++i) {
        for (int j(1); j < 9; ++j) {
            for (int k(1); k < 9; ++k) {
                std::array<double, 3> const v(field.velocity(i, j, k));
                CHECK_NEAR(v[0], Physics::V_INFINITY, 1e-9);
                CHECK_NEAR(v[1], 0.0, 1e-9);
                CHECK_NEAR(v[2], 0.0, 1e-9);
            }
        }
    }
}

void testSolverOverMountain() {
    std::cout << "PotentialField (solver, over a mountain)" << std::endl;

    GaussianPeak peak(10.0, 10.0, 8.0, 3.0, 3.0);
    PotentialField field(20, 20, 20, 1.0);
    field.initialize(Physics::V_INFINITY, peak);
    field.computeLaplacians();
    field.solve();

    /* Put an obstacle in the way and the flow must respond: air is lifted
       somewhere, and it speeds up as it is squeezed over the summit.
       Without this, the flat-terrain test above would pass on a solver
       that simply never changed anything. */
    double maxVertical(0.0);
    double maxSpeed(0.0);
    bool finite(true);
    for (int i(1); i < 19; ++i) {
        for (int j(1); j < 19; ++j) {
            for (int k(1); k < 19; ++k) {
                std::array<double, 3> const v(field.velocity(i, j, k));
                for (double c : v)
                    if (std::isnan(c) or std::isinf(c))
                        finite = false;
                maxVertical = std::max(maxVertical, std::abs(v[2]));
                maxSpeed = std::max(maxSpeed, std::abs(v[0]));
            }
        }
    }
    CHECK(finite);
    CHECK(maxVertical > 1.0);              // the air really is lifted
    CHECK(maxSpeed > Physics::V_INFINITY); // and accelerated over the peak
}

// --------------------------------------------- the whole pipeline, end to end

void testEndToEnd() {
    std::cout << "End to end (the reference configuration)" << std::endl;

    /* The configuration both front-ends ship with. This pins the whole
       chain, terrain to field to sky, against known counts, so any change
       that silently perturbs the physics shows up here. */
    PotentialField field(30, 30, 30, 20.0 / 29.0);
    std::vector<std::unique_ptr<Mountain>> chain;
    chain.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(15.0, 15.0, 15.0, 5.0, 5.0)));
    MountainChain terrain(chain);

    field.initialize(Physics::V_INFINITY, terrain);
    field.computeLaplacians();
    field.solve();

    Sky sky(field);
    sky.update();

    std::array<int, 3> const n(sky.getCellCount());
    int interior(0), cloudy(0), buried(0);
    for (int i(1); i < n[0] - 1; ++i) {
        for (int j(1); j < n[1] - 1; ++j) {
            for (int k(1); k < n[2] - 1; ++k) {
                ++interior;
                if (sky.isCloudy(i, j, k))
                    ++cloudy;
                if (sky.isBelowTerrain(i, j, k))
                    ++buried;
            }
        }
    }

    CHECK(interior == 21952);
    CHECK(cloudy == 361);
    CHECK(buried == 2002);

    // The sky settles: a further step does not move the cloud count.
    sky.update();
    int stable(0);
    for (int i(1); i < n[0] - 1; ++i)
        for (int j(1); j < n[1] - 1; ++j)
            for (int k(1); k < n[2] - 1; ++k)
                if (sky.isCloudy(i, j, k))
                    ++stable;
    CHECK(stable == cloudy);
}

// ------------------------------------------------ grid: non-cubic dimensions

void testNonCubicGrid() {
    std::cout << "Grid3D (non-cubic)" << std::endl;

    /* Grid3D nests its vectors so that cells[i][j][k] runs i along x,
       j along y and k along z. A cubic grid hides any mix-up between the
       axes, so this uses three different extents on purpose: reading every
       cell of it would go out of bounds if the nesting were transposed. */
    Sky sky(4.0, 8.0, 12.0, 1.0, Physics::V_INFINITY);
    std::array<int, 3> const n(sky.getCellCount());

    CHECK(n[0] == 5);
    CHECK(n[1] == 9);
    CHECK(n[2] == 13);
    CHECK(n[0] != n[1] and n[1] != n[2]);

    sky.update();
    int visited(0);
    for (int i(0); i < n[0]; ++i)
        for (int j(0); j < n[1]; ++j)
            for (int k(0); k < n[2]; ++k) {
                (void) sky.isCloudy(i, j, k);
                (void) sky.isBelowTerrain(i, j, k);
                ++visited;
            }
    CHECK(visited == n[0] * n[1] * n[2]);
}

// --------------------------------------- rendering: double dispatch (Visitor)

namespace {
/// A fake renderer that counts the calls it receives.
class SpyRenderer : public Renderer {
public:
    int mountains = 0;
    int skies = 0;
    void draw(Mountain const&) override { ++mountains; }
    void draw(Sky const&) override { ++skies; }
};
} // namespace

void testDoubleDispatch() {
    std::cout << "Double dispatch (Visitor)" << std::endl;

    SpyRenderer spy;

    GaussianPeak peak(1.0, 1.0, 1.0, 1.0, 1.0);
    peak.drawOn(spy);
    CHECK(spy.mountains == 1);
    CHECK(spy.skies == 0);

    // A chain presents itself to the renderer as a mountain, not as a special
    // case. That is what lets a new view be added without touching the model.
    std::vector<std::unique_ptr<Mountain>> v;
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(2.0, 2.0, 2.0, 1.0, 1.0)));
    MountainChain chain(v);
    chain.drawOn(spy);
    CHECK(spy.mountains == 2);

    Sky sky(5.0, 5.0, 5.0, 1.0, Physics::V_INFINITY);
    sky.drawOn(spy);
    CHECK(spy.skies == 1);
}

// ------------------------------------------------ rendering: Drawable is usable

void testDrawablePolymorphism() {
    std::cout << "Drawable (polymorphism)" << std::endl;

    /* Drawable only earns its place if callers can hold one without
       knowing the concrete type. Everything drawable therefore derives
       from it publicly, and drawOn dispatches to the right overload of
       Renderer through a base pointer. */
    SpyRenderer spy;

    GaussianPeak peak(3.0, 3.0, 5.0, 2.0, 2.0);
    Sky sky(4.0, 4.0, 4.0, 1.0, Physics::V_INFINITY);

    std::vector<Drawable*> scene{&peak, &sky};
    for (Drawable* item : scene)
        item->drawOn(spy);

    CHECK(spy.mountains == 1);
    CHECK(spy.skies == 1);
}

// ------------------------------------------- rendering: injected TextRenderer

void testTextRenderer() {
    std::cout << "TextRenderer (injected stream)" << std::endl;

    std::ostringstream out;
    TextRenderer renderer(out);

    GaussianPeak peak(15.0, 15.0, 15.0, 5.0, 5.0);
    peak.drawOn(renderer);

    std::string const text(out.str());
    CHECK(not text.empty());
    CHECK(text.find("A Gaussian peak") != std::string::npos);
    CHECK(text.find("height: 15") != std::string::npos);
}

// -------------------------------------------------------------------- main

int main() {
    testGaussianPeak();
    testMountainChain();
    testAirCell();
    testSky();
    testPotentialField();
    testNonCubicGrid();
    testSolverOnFlatTerrain();
    testSolverOverMountain();
    testEndToEnd();
    testDoubleDispatch();
    testDrawablePolymorphism();
    testTextRenderer();

    std::cout << std::endl << total - failures << " / " << total << " checks passed" << std::endl;

    if (failures > 0) {
        std::cout << failures << " FAILURE(S)" << std::endl;
        return 1;
    }
    std::cout << "All green." << std::endl;
    return 0;
}
