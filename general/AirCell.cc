#include "AirCell.h"
#include "constants.h"
#include <cmath> //pow + exp
#include <iostream>
using namespace Physics;

/**
 * @brief Sets the altitude of this cell.
 * @param z_ altitude
 */ 

void AirCell::setZ(double z_){
    z = z_;
}

/**
 * @brief Sets the cell to its initial physical state.
 * @param n true when the cell lies below the terrain
 */ 
void AirCell::initialize(bool n){
    update();
    belowTerrain = n;
}

void AirCell::update(){
    enthalpy = BERNOULLI_CONSTANT - G * z - velocitySquaredNorm()/2.0;
    temperature = (2.0/7)*M_DRY_AIR*enthalpy/R;
    humidity = TAU_INFINITY;
    pressure = P_INFINITY*std::pow(T_INFINITY, -7/2.0)*std::pow(temperature, 7/2.0);
    vapourPressure = (humidity*pressure)/((M_WATER/M_DRY_AIR) + humidity);
    saturationPressure = P_REF*std::exp(13.96-(5208/temperature));
}


/**
 * @brief Sets the wind velocity of this cell.
 * @param v velocity, as returned by PotentialField::velocity(i, j, k)
 */ 
void AirCell::setVelocity(std::array<double, 3> const& v){
    velocity = v;
}

/**
 * @brief Whether water vapour has condensed in this cell.
 * @return true when the vapour pressure exceeds saturation
 */ 
bool AirCell::isCloudy() const {
    return (vapourPressure > saturationPressure);
}

/**
 * @brief Squared norm of the velocity vector.
 */ 
double AirCell::velocitySquaredNorm() const{
    return (velocity[0]*velocity[0] + velocity[1]*velocity[1] + velocity[2]*velocity[2]);
}

/**
 * @brief Prints every quantity of this cell, in the order
 * v2 h T P Pvapour Psaturation cloud
 */
void AirCell::printState() const{
    std::cout << velocitySquaredNorm() << " " << enthalpy  << " " << temperature << " " << pressure << " " <<vapourPressure << " " << saturationPressure << " " << isCloudy();
}

std::ostream& AirCell::printState(std::ostream& out) const{
    out << velocitySquaredNorm() << " " << enthalpy  << " " << temperature << " " << pressure << " " <<vapourPressure << " " << saturationPressure << " " << isCloudy();
    return out;
}
