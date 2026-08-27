
#include <iostream>
#include <iomanip>
#include <vector>
#include "Vector2D.h"
#include "Potential.h"

/**
 * @brief Discrete Laplacian of the potential at this point.
 * 
 * Naming: i, j, k is the axis that differs from the current cell;
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
 * @brief Prints the potential.
 * Uses setprecision(14).
 */
void Potential::printPotential() const {
    std::cout <<std::setprecision(14) <<potential;
}

std::ostream& Potential::printPotential(std::ostream& out) const {
    out <<std::setprecision(14) <<potential;
    return out;
}
/**
 * @brief Prints the Laplacian, with setprecision(15).
 * setprecision = 15
 */
void Potential::printLaplacian() const {
    std::cout <<std::setprecision(15) <<laplacian;
}

/**
 * @brief Squared Euclidean norm of the Laplacian.
 * 
 * @return the squared norm
 */
double Potential::laplacianSquaredNorm() const{
    return laplacian.squaredNorm();
}

/**
 * @brief Whether the potential vector is zero, meaning the cell sits
 * 
 * below the terrain and is excluded from the solve.
 */
bool Potential::isZero() const{
    if(potential == Vector2D()) return true;
    return false;
}

/**
 * @brief Relaxation step: nudges the potential along its Laplacian.
 */
void Potential::iterate(const double eps){
        potential += eps * laplacian;
}

/**
 * @brief Velocity at a point, from the potentials of its neighbours.
 * 
 * The velocity is passed by reference to avoid allocating a new array.
 * 
 * Naming: P is a potential; i, j, k is the axis that differs from the
 */
void Potential::velocity(std::array<double, 3>& velocity, Potential const& Pi_m1, Potential const& Pj_m1, Potential const& Pk_m1,
    Potential const& Pi_p1, Potential const& Pj_p1, Potential const& Pk_p1) const {
        velocity[0] = Pj_p1.potential.getY() - Pj_m1.potential.getY() - Pk_p1.potential.getX() + Pk_m1.potential.getX();
        velocity[1] = -Pi_p1.potential.getY() + Pi_m1.potential.getY();
        velocity[2] = Pi_p1.potential.getX() - Pi_m1.potential.getX();
    }
