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

#include "Ciel.h"
#include "ChaineDeMontagnes.h"
#include "CubedAir.h"
#include "Gaussienne.h"
#include "SupportADessin.h"
#include "TextViewer.h"
#include "constantes.h"

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

// ------------------------------------------------------- relief : Gaussienne

void testGaussienne()
{
    std::cout << "Gaussienne" << std::endl;
    Gaussienne g(15.0, 15.0, 15.0, 5.0, 5.0);

    // Au sommet, l'altitude vaut exactement le paramètre de hauteur.
    VERIFIE_PROCHE(g.altitude(15.0, 15.0), 15.0, 1e-12);

    // La cloche décroît quand on s'éloigne du sommet.
    VERIFIE(g.altitude(15.0, 15.0) > g.altitude(18.0, 15.0));
    VERIFIE(g.altitude(18.0, 15.0) > g.altitude(21.0, 15.0));

    // Symétrie autour du sommet, sur les deux axes.
    VERIFIE_PROCHE(g.altitude(12.0, 15.0), g.altitude(18.0, 15.0), 1e-12);
    VERIFIE_PROCHE(g.altitude(15.0, 12.0), g.altitude(15.0, 18.0), 1e-12);

    // Loin du sommet, le seuil de 0.5 ramène l'altitude à zéro (plaine plate).
    VERIFIE_PROCHE(g.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------- relief : Composite (la chaîne)

void testChaineDeMontagnes()
{
    std::cout << "ChaineDeMontagnes (Composite)" << std::endl;

    std::vector<std::unique_ptr<Montagne>> v;
    v.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(10.0, 10.0, 12.0, 3.0, 3.0)));
    v.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(30.0, 30.0, 8.0, 3.0, 3.0)));
    ChaineDeMontagnes chaine(v);

    // La chaîne prend le maximum de ses composantes en chaque point.
    VERIFIE_PROCHE(chaine.altitude(10.0, 10.0), 12.0, 1e-12);
    VERIFIE_PROCHE(chaine.altitude(30.0, 30.0), 8.0, 1e-12);

    // Le constructeur déplace les pointeurs : le vecteur source est vidé.
    VERIFIE(chaine.getChaine().size() == 2);

    /* Le coeur du Composite : une chaîne EST une montagne, donc une chaîne
       peut en contenir une autre et le tout se comporte identiquement. */
    std::vector<std::unique_ptr<Montagne>> interne;
    interne.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(5.0, 5.0, 20.0, 2.0, 2.0)));

    std::vector<std::unique_ptr<Montagne>> externe;
    externe.emplace_back(std::unique_ptr<Montagne>(new ChaineDeMontagnes(interne)));
    externe.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(40.0, 40.0, 6.0, 2.0, 2.0)));
    ChaineDeMontagnes imbriquee(externe);

    VERIFIE_PROCHE(imbriquee.altitude(5.0, 5.0), 20.0, 1e-12);
    VERIFIE_PROCHE(imbriquee.altitude(40.0, 40.0), 6.0, 1e-12);
    VERIFIE_PROCHE(imbriquee.altitude(200.0, 200.0), 0.0, 1e-12);
}

// ------------------------------------------------ thermodynamique : CubedAir

void testCubedAir()
{
    std::cout << "CubedAir (thermodynamique)" << std::endl;
    using namespace Physique;

    /* Au sol, à la vitesse du vent au loin, on doit retrouver exactement
       les conditions à l'infini : c'est le cas limite analytique. */
    CubedAir sol;
    sol.setz(0.0);
    sol.setVitesse({v_infini, 0.0, 0.0});
    sol.initialise(false);

    VERIFIE_PROCHE(sol.getTemperature(), T_infini, 1e-9);
    VERIFIE_PROCHE(sol.getPression(), P_infini, 1e-6);
    VERIFIE_PROCHE(sol.getEnthalpie(), h_infini, 1e-6);

    // Au sol l'air est sous-saturé : pas de nuage.
    VERIFIE(sol.getP_eau() < sol.getP_rosee());
    VERIFIE(not sol.formationNuage());

    // En s'élevant, l'air se détend et se refroidit.
    CubedAir haut;
    haut.setz(1000.0);
    haut.setVitesse({v_infini, 0.0, 0.0});
    haut.initialise(false);

    VERIFIE(haut.getTemperature() < sol.getTemperature());
    VERIFIE(haut.getPression() < sol.getPression());

    // Assez haut, la pression de vapeur dépasse la saturation : nuage.
    VERIFIE(haut.getP_eau() > haut.getP_rosee());
    VERIFIE(haut.formationNuage());

    // L'énergie cinétique se convertit en enthalpie : à z égal, plus le
    // vent est rapide, plus l'air est froid.
    CubedAir rapide;
    rapide.setz(0.0);
    rapide.setVitesse({2.0 * v_infini, 0.0, 0.0});
    rapide.initialise(false);
    VERIFIE(rapide.getTemperature() < sol.getTemperature());

    // Le drapeau "sous la montagne" est bien conservé.
    CubedAir enterre;
    enterre.initialise(true);
    VERIFIE(enterre.sousMontagne());
    VERIFIE(not sol.sousMontagne());
}

// ------------------------------------------------------ grille : Collection3D

void testCiel()
{
    std::cout << "Ciel (grille 3D)" << std::endl;

    Ciel ciel(20.0, 20.0, 20.0, 20.0/29.0, Physique::v_infini);
    std::array<int, 3> const n(ciel.getCubes());

    VERIFIE(n[0] == 30);
    VERIFIE(n[1] == 30);
    VERIFIE(n[2] == 30);

    /* Après évolution, chaque cube est dans un état physique cohérent :
       Nuageux() doit être interrogeable partout sans sortir de la grille. */
    ciel.evolue();
    int nuageux(0);
    for(int i(0); i < n[0]; ++i)
        for(int j(0); j < n[1]; ++j)
            for(int k(0); k < n[2]; ++k)
                if(ciel.Nuageux(i, j, k)) ++nuageux;

    // Sans relief, l'air ne monte pas : aucune condensation attendue.
    VERIFIE(nuageux == 0);
}

// ------------------------------------------- rendu : double dispatch (Visitor)

namespace {
/// Support de dessin factice qui compte les appels reçus.
class SupportEspion : public SupportADessin {
public:
    int montagnes = 0;
    int ciels = 0;
    void dessine(Montagne const&) override { ++montagnes; }
    void dessine(Ciel const&) override { ++ciels; }
};
}

void testDoubleDispatch()
{
    std::cout << "Double dispatch (Visitor)" << std::endl;

    SupportEspion espion;

    Gaussienne g(1.0, 1.0, 1.0, 1.0, 1.0);
    g.dessine_sur(espion);
    VERIFIE(espion.montagnes == 1);
    VERIFIE(espion.ciels == 0);

    // Une chaîne se présente au support comme une montagne, pas comme un cas
    // particulier : c'est ce qui permet d'ajouter un rendu sans toucher au modèle.
    std::vector<std::unique_ptr<Montagne>> v;
    v.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(2.0, 2.0, 2.0, 1.0, 1.0)));
    ChaineDeMontagnes chaine(v);
    chaine.dessine_sur(espion);
    VERIFIE(espion.montagnes == 2);

    Ciel ciel(5.0, 5.0, 5.0, 1.0, Physique::v_infini);
    ciel.dessine_sur(espion);
    VERIFIE(espion.ciels == 1);
}

// -------------------------------------------------- rendu : TextViewer injecté

void testTextViewer()
{
    std::cout << "TextViewer (flot injecté)" << std::endl;

    std::ostringstream sortie;
    TextViewer vue(sortie);

    Gaussienne g(15.0, 15.0, 15.0, 5.0, 5.0);
    g.dessine_sur(vue);

    std::string const texte(sortie.str());
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
