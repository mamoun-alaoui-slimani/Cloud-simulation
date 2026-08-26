#include "AirCell.h"
#include "constants.h"
#include <cmath> //pow + exp
#include <iostream>
using namespace Physics;

/**
 * @brief Permet de donner au AirCell son altitude
 * @param z_ altitude du AirCell 
 */ 

void AirCell::setZ(double z_){
    z = z_;
}

/**
 * @brief La méthode initialize permet de d'initialiser tous les attributs privés du AirCell (par défaut tous à 0 quand construit) avec les conditions initiales données dans le complément mathématique.
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
 * @brief Setter pour la velocity du AirCell
 * @param v std::array<double,3>, obtenue par la méthode PotentialField::velocity(i, j, k) où i,j,k sont les indices du AirCell.
 */ 
void AirCell::setVelocity(std::array<double, 3> const& v){
    velocity = v;
}

/**
 * @brief Determine si le AirCell est une cellule nuageuse ou non
 * @return true (1) si cellule nuageuse, false (0) sinon
 */ 
bool AirCell::isCloudy() const {
    return (vapourPressure > saturationPressure);
}

/**
 * @brief Calcule la norm au carré du vecteur de velocity du AirCell.
 */ 
double AirCell::velocitySquaredNorm() const{
    return (velocity[0]*velocity[0] + velocity[1]*velocity[1] + velocity[2]*velocity[2]);
}

/**
 * @brief Permet l'affichage de toutes les informations relatives à l'instance courante selon le format
 * V2 h T P Peau Prosee Nuage
 */ 
void AirCell::printState() const{
    std::cout << velocitySquaredNorm() << " " << enthalpy  << " " << temperature << " " << pressure << " " <<vapourPressure << " " << saturationPressure << " " << isCloudy();
}

std::ostream& AirCell::printState(std::ostream& out) const{
    out << velocitySquaredNorm() << " " << enthalpy  << " " << temperature << " " << pressure << " " <<vapourPressure << " " << saturationPressure << " " << isCloudy();
    return out;
}
