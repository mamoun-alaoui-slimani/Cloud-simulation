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
#include "GaussianPeak.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "constants.h"

// ------------------------------------------------------------------ harness

namespace {
int total(0);
int failures(0);

void check(bool condition, std::string const& description, int line)
{
    ++total;
    if(not condition){
        ++failures;
        std::cerr << "  FAILED (line " << line << ") : " << description << std::endl;
    }
}

void checkNear(double actual, double expected, double tolerance,
                   std::string const& description, int line)
{
    ++total;
    if(std::abs(actual - expected) > tolerance){
        ++failures;
        std::cerr << "  FAILED (line " << line << ") : " << description
                  << " ; got " << actual << ", expected " << expected << std::endl;
    }
}
}

#define CHECK(cond) check((cond), #cond, __LINE__)
#define CHECK_NEAR(actual, expected, tol) \
    checkNear((actual), (expected), (tol), #actual " ~= " #expected, __LINE__)

// ------------------------------------------------------- relief : GaussianPeak

void testGaussienne()
{
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

void testChaineDeMontagnes()
{
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

void testCubedAir()
{
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

void testCiel()
{
    std::cout << "Sky (3D grid)" << std::endl;

    Sky sky(20.0, 20.0, 20.0, 20.0/29.0, Physics::V_INFINITY);
    std::array<int, 3> const n(sky.getCellCount());

    CHECK(n[0] == 30);
    CHECK(n[1] == 30);
    CHECK(n[2] == 30);

    /* After the update every cell holds a coherent physical state, and
       isCloudy() must be callable everywhere without leaving the grid. */
    sky.update();
    int nuageux(0);
    for(int i(0); i < n[0]; ++i)
        for(int j(0); j < n[1]; ++j)
            for(int k(0); k < n[2]; ++k)
                if(sky.isCloudy(i, j, k)) ++nuageux;

    // With no terrain the air does not rise, so no condensation.
    CHECK(nuageux == 0);
}

// ------------------------------------------------ potential field: bounds

void testChampPotentiels()
{
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
    (void)field.isPotentialZero(5, 5, 5);
}

// --------------------------------------- rendering: double dispatch (Visitor)

namespace {
/// A fake renderer that counts the calls it receives.
class SupportEspion : public Renderer {
public:
    int montagnes = 0;
    int ciels = 0;
    void draw(Mountain const&) override { ++montagnes; }
    void draw(Sky const&) override { ++ciels; }
};
}

void testDoubleDispatch()
{
    std::cout << "Double dispatch (Visitor)" << std::endl;

    SupportEspion espion;

    GaussianPeak peak(1.0, 1.0, 1.0, 1.0, 1.0);
    peak.drawOn(espion);
    CHECK(espion.montagnes == 1);
    CHECK(espion.ciels == 0);

    // A chain presents itself to the renderer as a mountain, not as a special
    // case. That is what lets a new view be added without touching the model.
    std::vector<std::unique_ptr<Mountain>> v;
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(2.0, 2.0, 2.0, 1.0, 1.0)));
    MountainChain chain(v);
    chain.drawOn(espion);
    CHECK(espion.montagnes == 2);

    Sky sky(5.0, 5.0, 5.0, 1.0, Physics::V_INFINITY);
    sky.drawOn(espion);
    CHECK(espion.ciels == 1);
}

// ------------------------------------------- rendering: injected TextRenderer

void testTextViewer()
{
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

int main()
{
    testGaussienne();
    testChaineDeMontagnes();
    testCubedAir();
    testCiel();
    testChampPotentiels();
    testDoubleDispatch();
    testTextViewer();

    std::cout << std::endl
              << total - failures << " / " << total << " checks passed"
              << std::endl;

    if(failures > 0){
        std::cout << failures << " FAILURE(S)" << std::endl;
        return 1;
    }
    std::cout << "All green." << std::endl;
    return 0;
}
