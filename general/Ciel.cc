
#include "Ciel.h"
#include "ChampPotentiels.h"
#include <cmath>

/**
 * @brief Constructeur de Ciel demandé. Initialise tous les cubes d'air selon les conditions initiales et donne une vitesse du vent uniforme selon une direction (x).
 * @param Lx longueur de la boîte dans la direction x
 * @param Ly longueur de la boîte dans la direction y
 * @param Lz longueur de la boîte dans la direction z
 * @param lambda pas d'échantillonage
 * @param vitesse première composante de la vitesse des cubes d'air.
 */ 
Ciel::Ciel(double Lx, double Ly, double Lz, double lambda, double vitesse):
Collection3D<CubedAir>::Collection3D(ceil(1 + (Lx/lambda)), ceil(1 + (Ly/lambda)), ceil(1 + (Lz/lambda)), lambda){
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                collection3D[i][j][k].setz(taille_cube * k);
                collection3D[i][j][k].setVitesse(std::array<double, 3>({vitesse, 0.0, 0.0}));
            }
        }
    }
}

/**
 * @brief Met à jour les valeurs contenues dans les CubedAir du Ciel
 * 
 */
void Ciel::evolue(){
    for(std::vector<std::vector<CubedAir>>& vec2D : collection3D){
        for(std::vector<CubedAir>& vec1D : vec2D){
            for(CubedAir& cube : vec1D){
                cube.evolue();
            }
        }
    }
}

Ciel::Ciel() : Collection3D<CubedAir>::Collection3D<CubedAir>() {}

/**
 * @brief Constructeur de ciel se basant sur une instance déjà créée de ChampPotentiels. Initialise aussi les attributs avec les conditions initiales.
 * @param cp Instance de ChampPotentiel
 */ 
Ciel::Ciel(ChampPotentiels const& cp)
: Collection3D::Collection3D(cp.getNbrCubes()[0], cp.getNbrCubes()[1], cp.getNbrCubes()[2], cp.getTailleCube()) {
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                collection3D[i][j][k].setz(taille_cube * k);
                collection3D[i][j][k].setVitesse(cp.vitesse(i,j,k));
                collection3D[i][j][k].initialise(cp.potentielNul(i,j,k));
            }
        }
    }
}

/**
 * @brief Dessine le Ciel sur un support
 * 
 * @param support support sur lequel dessiner le Ciel (Texte, graphique, écran, fichier...)
 */
void Ciel::dessine_sur(SupportADessin& support){
    support.dessine(*this);
}

/**
 * @brief Indique si le cube d'air en (i, j, k) est nuageux
 * @param i, j, k indices du cube dans la grille
 * @return true si la vapeur d'eau y a condensé
 */
bool Ciel::Nuageux(int i, int j, int k) const{
    return collection3D[i][j][k].formationNuage();
}

/**
 * @brief Indique si le cube d'air en (i, j, k) est situé sous le relief
 * @param i, j, k indices du cube dans la grille
 * @return true si le cube est enterré dans la montagne (donc pas à dessiner)
 */
bool Ciel::SousMontagne(int i, int j, int k) const{
    return collection3D[i][j][k].sousMontagne();
}

/**
 * @brief Affiche les Nuages sur un flot de sortie
 * 
 * @param sortie flot de sortie
 * @param m Montagne à utiliser pour évaluer la formation de Nuage
 * @return flot de sortie modifié
 */
std::ostream& Ciel::afficheNuages(std::ostream& sortie) const{ //ne devrait pas prendre la Montagne en paramètre car pas accès lors de l'affichage avec TextViewer
    sortie <<"Les nuages du ciel : " <<std::endl;
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                if(not(i == 0 or j == 0 or i == nbr_cubes[0] - 1 or j == nbr_cubes[1] - 1 or k == 0 or k == nbr_cubes[2]-1)){
                    sortie << i << " " << j <<" " << k <<" ";
                    collection3D[i][j][k].afficheNuage(sortie);
                    sortie << " " <<std::endl;
                }
            }
        }
    }
    return sortie;
}

std::ostream& operator<<(std::ostream& sortie, Ciel const& ciel){
    return ciel.afficheNuages(sortie);
}