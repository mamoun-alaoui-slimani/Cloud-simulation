
#include <iostream>
#include <iomanip>
#include <vector>
#include "Vecteur2D.h"
#include "Potentiel.h"

/**
 * @brief Calcul le laplacien lié à un vecteur potentiel. 
 * 
 * Notation i,j,k : indice différent de celui de l'instance courante m1,p1: moins ou plus 1
 * 
 * @param Vi_m1 
 * @param Vj_m1 
 * @param Vk_m1 
 * @param Vi_p1 
 * @param Vj_p1 
 * @param Vk_p1 
 */
void Potentiel::calcule_laplacien(Potentiel const& Vi_m1, Potentiel const& Vj_m1, Potentiel const& Vk_m1,
Potentiel const& Vi_p1, Potentiel const& Vj_p1, Potentiel const& Vk_p1){
    laplacien = Vi_m1.potentiel + Vj_m1.potentiel + Vk_m1.potentiel - 6*potentiel + Vi_p1.potentiel + Vj_p1.potentiel + Vk_p1.potentiel;
}

/**
 * @brief afficher le Potentiel.
 * setprecision à 14 
 */
void Potentiel::affichePotentiel() const {
    std::cout <<std::setprecision(14) <<potentiel;
}

std::ostream& Potentiel::affichePotentiel(std::ostream& sortie) const {
    sortie <<std::setprecision(14) <<potentiel;
    return sortie;
}
/**
 * @brief afficher le Laplacien. 
 * setprecision = 15
 */
void Potentiel::afficheLaplacien() const {
    std::cout <<std::setprecision(15) <<laplacien;
}

/**
 * @brief Donne la norme Euclidienne du Laplacien
 * 
 * @return double norme euclidienne du Laplacien
 */
double Potentiel::Norme2Laplacien() const{
    return laplacien.norme2();
}

/**
 * @brief Test si le Vecteur2D potentiel est nul ou pas
 * 
 * @return true si nul, false sinon
 */
bool Potentiel::PotentielNul() const{
    if(potentiel == Vecteur2D()) return true;
    return false;
}

/**
 * @brief Recalcule de l'attribut potentiel pour la résolution des équations de Laplace.
 */
void Potentiel::iteration(const double eps){
        potentiel += eps * laplacien;
}

/**
 * @brief Calcule la vitesse d'un point de la boîte grâce aux potentiels alentours. 
 * 
 * La vitesse est passée en référence pour éviter de créer une nouvelle instance de std::vector<double>. 
 * 
 * Notation: P : Potentiel i,j,k: indice changé par rapport à celui de l'instance courante m1, p1: moins ou plus 1 sur l'indice
 */
void Potentiel::vitesse(std::array<double, 3>& vitesse, Potentiel const& Pi_m1, Potentiel const& Pj_m1, Potentiel const& Pk_m1,
    Potentiel const& Pi_p1, Potentiel const& Pj_p1, Potentiel const& Pk_p1) const {
        vitesse[0] = Pj_p1.potentiel.getY() - Pj_m1.potentiel.getY() - Pk_p1.potentiel.getX() + Pk_m1.potentiel.getX();
        vitesse[1] = -Pi_p1.potentiel.getY() + Pi_m1.potentiel.getY();
        vitesse[2] = Pi_p1.potentiel.getX() - Pi_m1.potentiel.getX();
    }
