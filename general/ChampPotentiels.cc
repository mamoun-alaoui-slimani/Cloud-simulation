
#include <array>
#include <iostream>
#include <iomanip> 
#include "ChampPotentiels.h"

/**
 * @brief Construit un nouvel objet ChampsPotentiels. 
 * Appelle le constructeur de Collection3D.
 * 
 * @param Nx nombres de cubes désirés sur l'axe des x
 * @param Ny nombres de cubes désirés sur l'axe des y
 * @param Nz nombres de cubes désirés sur l'axe des z
 * @param lambda Taille des cubes
 */
ChampPotentiels::ChampPotentiels(int Nx, int Ny, int Nz, double lambda)
:Collection3D<Potentiel>::Collection3D(Nx, Ny, Nz, lambda) {} 

/**
 * @brief Initialise le champs de potentiels.
 * Les potentiels sont intialisés à (0,0) si sous la montagne.
 * Les laplaciens sont tous initialisés à (0,0).
 * 
 * @param v vitesse aux bords
 * @param m Montagne à utiliser pour initialiser le champs de potentiels
 */
void ChampPotentiels::initialise(double v, const Montagne &m){
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                if(k >= m.altitude(i, j) or i == 0 or j == 0 or i == nbr_cubes[0] - 1 or j == nbr_cubes[1] - 1) {
                    collection3D[i][j][k] = Potentiel(Vecteur2D(-v*(k*taille_cube)/2, v*(j*taille_cube - (longueurs[1]/2))/2), Vecteur2D());
                } else {
                    collection3D[i][j][k] = Potentiel(); 
                    //Normalement pas nécessaire puisque notre ChampPotentiel est construit avec les deux vecteurs de Potentiel initialisés à (0,0) mais pour le moment laisser au cas où on veuille réinitialiser un ChampPotentiel dans lequel les Potentiels ont été modifiés.
                }
            }
        }
    }
}

/**
 * @brief Calcul les Laplaciens pour chaque Potentiel.
 * 
 * Le calcul ne se fait pas si le point est sous la montagne.
 */
void ChampPotentiels::calcule_laplaciens(){
    for(int i(1); i < nbr_cubes[0] - 1; ++i){
        for(int j(1); j < nbr_cubes[1] - 1; ++j){
            for(int k(1); k < nbr_cubes[2] - 1; ++k){
                //Pour vérifier si sous la montagne ou non
                if(not collection3D[i][j][k].PotentielNul()){
                collection3D[i][j][k].calcule_laplacien(collection3D[i-1][j][k], collection3D[i][j-1][k], collection3D[i][j][k-1],
                collection3D[i+1][j][k], collection3D[i][j+1][k], collection3D[i][j][k+1]);
                }
            }
        }
    }
}

/**
 * @brief Affiche les potentiels du champs selon le format:
 * i j k P1 P2
 */
void ChampPotentiels::affiche_potentiels() const {
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                std::cout << i  <<" " << j <<" " << k <<" ";
                collection3D[i][j][k].affichePotentiel();
                //std::cout <<" " << std::endl; //Parfois nécessaire pour gnuplot
                std::cout << std::endl;
            }
        }
    }
}

/**
 * @brief Affiche les Laplaciens du champs selon le format:
 *  i j k L2 L3
 */
void ChampPotentiels::affiche_laplaciens() const {
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
               //if(not(i == 0 or j == 0 or i == nbr_cubes[0] - 1 or j == nbr_cubes[1] - 1 or k == 0 or k == nbr_cubes[2]-1)){
                std::cout << i << " " << j <<" " << k <<" ";
                collection3D[i][j][k].afficheLaplacien();
                std::cout << " " <<std::endl;
                //} //Partie commentée peut être décommentée pour éviter l'affichage au bord pour la comparaison avec les fichiers donnés en exemple.
            }
        }
    }
}
/**
 * @brief calcule l'erreur nécessaire pour déterminer si convergence.
 * @return double: Somme de la norme au carré de tous les laplaciens.
 */ 
double ChampPotentiels::erreur() const {
    double norme2(0.0);
    for(int i(0); i < nbr_cubes[0]; i++){
        for(int j(0); j < nbr_cubes[1]; j++){
            for(int k(0); k < nbr_cubes[2]; k++){
                norme2 += collection3D[i][j][k].Norme2Laplacien();
            }
        }
    }
    return norme2;
}

/**
 * @brief Appelle la méthode Potentiel::iteration() pour tous les points intérieurs de la boîte.
 */ 
void ChampPotentiels::iteration(const double eps){
    for(int i(1); i < nbr_cubes[0] -1; i++){
        for(int j(1); j < nbr_cubes[1] -1; j++){
            for(int k(1); k < nbr_cubes[2] -1; k++){
                collection3D[i][j][k].iteration(eps);
            }
        }
    }
}

/**
 * @brief Résoud les équations de Laplace. Tant que l'erreur obtenue est plus grande que le seuil ou que le nombre d'itération est plus petit que le nombre max: apppelle la méthode ChampPotentiels::iteration(). Puis recalcule les laplaciens avec les nouvelles valeurs des potentiels.
 * 
 * @param seuil seuil d'erreur pour déterminer si convergence ou non
 * @param IteMax nombre max d'itération pour éviter boucle infini
 * @param affiche si vrai la méthode affiche les potentiels et laplaciens à chaque nouvelle itération
 */ 
void ChampPotentiels::resolution(double seuil, int IteMax, bool affiche){
    int Ite(1);
    double erreur(this->erreur());
    while((seuil < erreur) and (Ite <= IteMax)){
        //std::cout << Ite <<" " <<erreur <<std::endl;
        this->iteration();
        this->calcule_laplaciens();
        if(affiche){
            this->affiche_potentiels(); //Si verbeuse
            this->affiche_laplaciens();
        }
        erreur = this->erreur(); //Actualisation de l'erreur avec les nouvelles valeurs des potentiels.
        Ite++;
    }
    //std::cout << Ite <<" " <<erreur <<std::endl;
}

/**
 * @brief Pour les indices donnés en paramètre, calcule la vitesse du vent associée au point. La vitesse au bord n'étant pas définie (sinon segmentation fault), on la met à (0,0,0).
 * @param i 
 * @param j 
 * @param k
 * 
 * @return Retourne un vecteur 3D où les composantes sont la vitesse vx, vy et vz respectivement.
 */ 
std::array<double, 3> ChampPotentiels::vitesse(int i, int j, int k) const{
    std::array<double, 3> vitesse({0,0,0}); //Array de 3 double nul

    if(i == 0 or j == 0 or k == 0 or i == nbr_cubes[0] - 1 or j == nbr_cubes[1] -1 or k == nbr_cubes[2] - 1) return vitesse;

    collection3D[i][j][k].vitesse(vitesse, collection3D[i-1][j][k], collection3D[i][j-1][k], collection3D[i][j][k-1], collection3D[i+1][j][k], collection3D[i][j+1][k], collection3D[i][j][k+1]);

    for(size_t i(0); i < vitesse.size(); ++i){
        vitesse[i] *= (1 / (2 * taille_cube));
    }
    return vitesse;
}

bool ChampPotentiels::potentielNul(int i, int j, int k) const {
    if(0 <= i and i <= nbr_cubes[0] and 0 <= j and j <= nbr_cubes[1] and 0 <= k and k <= nbr_cubes[2]){
        return collection3D[i][j][k].PotentielNul();
    }
    return true; //On considere le potentiel nul s'il est en dehors des limites de l'espace

}

/** @brief Affiche les indices du point, les composantes du vecteur potentiel, la vitesse et sa norme au carré (v2) pour tous les points de notre boîte selon le format suivant:
 * i j k P1 P2 vx vy vz v2 
 */ 
void ChampPotentiels::affiche_vitesses() const{
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                if(not collection3D[i][j][k].PotentielNul()){

                    std::cout << i << " " << j <<" " << k <<" ";
                    collection3D[i][j][k].affichePotentiel();

                    std::array<double, 3> vit(this->vitesse(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    std::cout <<std::setprecision(6) << " " <<vit[0] <<" " <<vit[1] <<" " <<vit[2] <<" " <<V2 <<" " <<std::endl;
                }
            }
        }
    }
}

/**
 * @brief Affiche les vitesses sur un flot de sortie
 * 
 * @param sortie flot de sortie
 * @return flot de sortie modifié
 */
std::ostream& ChampPotentiels::affiche_vitesses(std::ostream& sortie) const{
    for(int i(0); i < nbr_cubes[0]; ++i){
        for(int j(0); j < nbr_cubes[1]; ++j){
            for(int k(0); k < nbr_cubes[2]; ++k){
                if(not collection3D[i][j][k].PotentielNul()){

                    sortie << i << " " << j <<" " << k <<" ";
                    collection3D[i][j][k].affichePotentiel(sortie);

                    std::array<double, 3> vit(this->vitesse(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    sortie <<std::setprecision(6) << " " <<vit[0] <<" " <<vit[1] <<" " <<vit[2] <<" " <<V2 <<" " <<std::endl;
                }
            }
        }
    }
    return sortie;
}

/**
 * @brief Surcharge de l'opérateur d'affichage
 * 
 * @param sortie flot de sortie
 * @param champ Champs de Potentiels à afficher
 * @return flot de sortie modifié
 */
std::ostream& operator<<(std::ostream& sortie, ChampPotentiels const& champ){
    sortie <<"Le système est constitué d'un champ de Potentiel:  " <<std::endl;
    return champ.affiche_vitesses(sortie);
}

/**
 * @brief Getter pour l'attribut taille_cube
 */ 
double ChampPotentiels::getTailleCube() const {
    return taille_cube;
}

/**
 * @brief Getter pour l'attribut nbr_cubes
 */ 
std::array<int, 3> ChampPotentiels::getNbrCubes() const {
    return nbr_cubes;
}
