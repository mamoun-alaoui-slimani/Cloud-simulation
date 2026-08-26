/**
 * @file main_tests.cc
 * @brief Tests unitaires du modèle physique et des patrons de conception.
 *
 * Pas de dépendance externe : un compteur, deux macros, un code de retour
 * non nul si quoi que ce soit échoue (utilisable tel quel en CI).
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

// ---------------------------------------------------------------- outillage

namespace {
int total(0);
int echecs(0);

void verifie(bool condition, std::string const& description, int ligne)
{
    ++total;
    if(not condition){
        ++echecs;
        std::cerr << "  ECHEC (ligne " << ligne << ") : " << description << std::endl;
    }
}

void verifieProche(double obtenu, double attendu, double tolerance,
                   std::string const& description, int ligne)
{
    ++total;
    if(std::abs(obtenu - attendu) > tolerance){
        ++echecs;
        std::cerr << "  ECHEC (ligne " << ligne << ") : " << description
                  << " ; obtenu " << obtenu << ", attendu " << attendu << std::endl;
    }
}
}

#define VERIFIE(cond) verifie((cond), #cond, __LINE__)
#define VERIFIE_PROCHE(obtenu, attendu, tol) \
    verifieProche((obtenu), (attendu), (tol), #obtenu " ~= " #attendu, __LINE__)

// ------------------------------------------------------- relief : GaussianPeak

void testGaussienne()
{
    std::cout << "Gaussienne" << std::endl;
    GaussianPeak peak(15.0, 15.0, 15.0, 5.0, 5.0);

    // Au sommet, l'altitude vaut exactement le paramètre de height.
    VERIFIE_PROCHE(peak.altitude(15.0, 15.0), 15.0, 1e-12);

    // La cloche décroît quand on s'éloigne du sommet.
    VERIFIE(peak.altitude(15.0, 15.0) > peak.altitude(18.0, 15.0));
    VERIFIE(peak.altitude(18.0, 15.0) > peak.altitude(21.0, 15.0));

    // Symétrie autour du sommet, sur les deux axes.
    VERIFIE_PROCHE(peak.altitude(12.0, 15.0), peak.altitude(18.0, 15.0), 1e-12);
    VERIFIE_PROCHE(peak.altitude(15.0, 12.0), peak.altitude(15.0, 18.0), 1e-12);

    // Loin du sommet, le threshold de 0.5 ramène l'altitude à zéro (plaine plate).
    VERIFIE_PROCHE(peak.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------- relief : Composite (la chaîne)

void testChaineDeMontagnes()
{
    std::cout << "ChaineDeMontagnes (Composite)" << std::endl;

    std::vector<std::unique_ptr<Mountain>> v;
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(10.0, 10.0, 12.0, 3.0, 3.0)));
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(30.0, 30.0, 8.0, 3.0, 3.0)));
    MountainChain chain(v);

    // La chaîne prend le maximum de ses composantes en chaque point.
    VERIFIE_PROCHE(chain.altitude(10.0, 10.0), 12.0, 1e-12);
    VERIFIE_PROCHE(chain.altitude(30.0, 30.0), 8.0, 1e-12);

    // Le constructeur déplace les pointeurs : le vecteur source est vidé.
    VERIFIE(chain.getChain().size() == 2);

    /* Le coeur du Composite : une chaîne EST une mountain, donc une chaîne
       peut en contenir une autre et le tout se comporte identiquement. */
    std::vector<std::unique_ptr<Mountain>> interne;
    interne.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(5.0, 5.0, 20.0, 2.0, 2.0)));

    std::vector<std::unique_ptr<Mountain>> externe;
    externe.emplace_back(std::unique_ptr<Mountain>(new MountainChain(interne)));
    externe.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(40.0, 40.0, 6.0, 2.0, 2.0)));
    MountainChain imbriquee(externe);

    VERIFIE_PROCHE(imbriquee.altitude(5.0, 5.0), 20.0, 1e-12);
    VERIFIE_PROCHE(imbriquee.altitude(40.0, 40.0), 6.0, 1e-12);
    VERIFIE_PROCHE(imbriquee.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------------ thermodynamique : AirCell

void testCubedAir()
{
    std::cout << "CubedAir (thermodynamique)" << std::endl;
    using namespace Physics;

    /* Au sol, à la velocity du vent au loin, on doit retrouver exactement
       les conditions à l'infini : c'est le cas limite analytique. */
    AirCell sol;
    sol.setZ(0.0);
    sol.setVelocity({V_INFINITY, 0.0, 0.0});
    sol.initialize(false);

    VERIFIE_PROCHE(sol.getTemperature(), T_INFINITY, 1e-9);
    VERIFIE_PROCHE(sol.getPressure(), P_INFINITY, 1e-6);
    VERIFIE_PROCHE(sol.getEnthalpy(), H_INFINITY, 1e-6);

    // Au sol l'air est sous-saturé : pas de nuage.
    VERIFIE(sol.getVapourPressure() < sol.getSaturationPressure());
    VERIFIE(not sol.isCloudy());

    // En s'élevant, l'air se détend et se refroidit.
    AirCell haut;
    haut.setZ(1000.0);
    haut.setVelocity({V_INFINITY, 0.0, 0.0});
    haut.initialize(false);

    VERIFIE(haut.getTemperature() < sol.getTemperature());
    VERIFIE(haut.getPressure() < sol.getPressure());

    // Assez haut, la pressure de vapeur dépasse la saturation : nuage.
    VERIFIE(haut.getVapourPressure() > haut.getSaturationPressure());
    VERIFIE(haut.isCloudy());

    // L'énergie cinétique se convertit en enthalpy : à z égal, plus le
    // vent est rapide, plus l'air est froid.
    AirCell rapide;
    rapide.setZ(0.0);
    rapide.setVelocity({2.0 * V_INFINITY, 0.0, 0.0});
    rapide.initialize(false);
    VERIFIE(rapide.getTemperature() < sol.getTemperature());

    // Le drapeau "sous la montagne" est bien conservé.
    AirCell enterre;
    enterre.initialize(true);
    VERIFIE(enterre.isBelowTerrain());
    VERIFIE(not sol.isBelowTerrain());
}

// ------------------------------------------------------ grille : Grid3D

void testCiel()
{
    std::cout << "Ciel (grille 3D)" << std::endl;

    Sky sky(20.0, 20.0, 20.0, 20.0/29.0, Physics::V_INFINITY);
    std::array<int, 3> const n(sky.getCellCount());

    VERIFIE(n[0] == 30);
    VERIFIE(n[1] == 30);
    VERIFIE(n[2] == 30);

    /* Après évolution, chaque cube est dans un état physique cohérent :
       isCloudy() doit être interrogeable partout sans sortir de la grille. */
    sky.update();
    int nuageux(0);
    for(int i(0); i < n[0]; ++i)
        for(int j(0); j < n[1]; ++j)
            for(int k(0); k < n[2]; ++k)
                if(sky.isCloudy(i, j, k)) ++nuageux;

    // Sans relief, l'air ne monte pas : aucune condensation attendue.
    VERIFIE(nuageux == 0);
}

// ------------------------------------------ field de potentiels : bornes

void testChampPotentiels()
{
    std::cout << "ChampPotentiels (bornes)" << std::endl;

    PotentialField field(6, 6, 6, 1.0);
    GaussianPeak relief(3.0, 3.0, 4.0, 1.0, 1.0);
    field.initialize(Physics::V_INFINITY, relief);

    VERIFIE(field.getCellCount()[0] == 6);
    VERIFIE_PROCHE(field.getCellSize(), 1.0, 1e-12);

    /* Tout indice hors de la boite doit etre traite comme un potential nul,
       sans jamais indexer hors bornes. L'indice N (et non N-1) est le cas
       limite : la garde utilisait <= et le laissait passer. */
    VERIFIE(field.isPotentialZero(6, 0, 0));
    VERIFIE(field.isPotentialZero(0, 6, 0));
    VERIFIE(field.isPotentialZero(0, 0, 6));
    VERIFIE(field.isPotentialZero(-1, 0, 0));
    VERIFIE(field.isPotentialZero(0, -1, 0));
    VERIFIE(field.isPotentialZero(0, 0, -1));
    VERIFIE(field.isPotentialZero(1000, 1000, 1000));

    // Un indice valide reste interrogeable normalement.
    (void)field.isPotentialZero(5, 5, 5);
}

// ------------------------------------------- rendu : double dispatch (Visitor)

namespace {
/// Support de dessin factice qui compte les appels reçus.
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
    VERIFIE(espion.montagnes == 1);
    VERIFIE(espion.ciels == 0);

    // Une chaîne se présente au renderer comme une mountain, pas comme un cas
    // particulier : c'est ce qui permet d'ajouter un rendu sans toucher au modèle.
    std::vector<std::unique_ptr<Mountain>> v;
    v.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(2.0, 2.0, 2.0, 1.0, 1.0)));
    MountainChain chain(v);
    chain.drawOn(espion);
    VERIFIE(espion.montagnes == 2);

    Sky sky(5.0, 5.0, 5.0, 1.0, Physics::V_INFINITY);
    sky.drawOn(espion);
    VERIFIE(espion.ciels == 1);
}

// -------------------------------------------------- rendu : TextRenderer injecté

void testTextViewer()
{
    std::cout << "TextViewer (flot injecté)" << std::endl;

    std::ostringstream out;
    TextRenderer renderer(out);

    GaussianPeak peak(15.0, 15.0, 15.0, 5.0, 5.0);
    peak.drawOn(renderer);

    std::string const texte(out.str());
    VERIFIE(not texte.empty());
    VERIFIE(texte.find("Gaussienne") != std::string::npos);
    VERIFIE(texte.find("hauteur: 15") != std::string::npos);
}

// ---------------------------------------------------------------------- main

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
              << total - echecs << " / " << total << " verifications passees"
              << std::endl;

    if(echecs > 0){
        std::cout << echecs << " ECHEC(S)" << std::endl;
        return 1;
    }
    std::cout << "Tout est vert." << std::endl;
    return 0;
}
