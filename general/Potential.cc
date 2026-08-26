
#include <iostream>
#include <iomanip>
#include <vector>
#include "Vector2D.h"
#include "Potential.h"

/**
 * @brief Calcul le laplacian lié à un vecteur potential. 
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
void Potential::computeLaplacian(Potential const& Vi_m1, Potential const& Vj_m1, Potential const& Vk_m1,
Potential const& Vi_p1, Potential const& Vj_p1, Potential const& Vk_p1){
    laplacian = Vi_m1.potential + Vj_m1.potential + Vk_m1.potential - 6*potential + Vi_p1.potential + Vj_p1.potential + Vk_p1.potential;
}

/**
 * @brief afficher le Potential.
 * setprecision à 14 
 */
void Potential::printPotential() const {
    std::cout <<std::setprecision(14) <<potential;
}

std::ostream& Potential::printPotential(std::ostream& out) const {
    out <<std::setprecision(14) <<potential;
    return out;
}
/**
 * @brief afficher le Laplacien. 
 * setprecision = 15
 */
void Potential::printLaplacian() const {
    std::cout <<std::setprecision(15) <<laplacian;
}

/**
 * @brief Donne la norm Euclidienne du Laplacien
 * 
 * @return double norm euclidienne du Laplacien
 */
double Potential::laplacianSquaredNorm() const{
    return laplacian.squaredNorm();
}

/**
 * @brief Test si le Vector2D potential est nul ou pas
 * 
 * @return true si nul, false sinon
 */
bool Potential::isZero() const{
    if(potential == Vector2D()) return true;
    return false;
}

/**
 * @brief Recalcule de l'attribut potential pour la résolution des équations de Laplace.
 */
void Potential::iterate(const double eps){
        potential += eps * laplacian;
}

/**
 * @brief Calcule la velocity d'un point de la boîte grâce aux potentiels alentours. 
 * 
 * La velocity est passée en référence pour éviter de créer une nouvelle instance de std::vector<double>. 
 * 
 * Notation: P : Potential i,j,k: indice changé par rapport à celui de l'instance courante m1, p1: moins ou plus 1 sur l'indice
 */
void Potential::velocity(std::array<double, 3>& velocity, Potential const& Pi_m1, Potential const& Pj_m1, Potential const& Pk_m1,
    Potential const& Pi_p1, Potential const& Pj_p1, Potential const& Pk_p1) const {
        velocity[0] = Pj_p1.potential.getY() - Pj_m1.potential.getY() - Pk_p1.potential.getX() + Pk_m1.potential.getX();
        velocity[1] = -Pi_p1.potential.getY() + Pi_m1.potential.getY();
        velocity[2] = Pi_p1.potential.getX() - Pi_m1.potential.getX();
    }
