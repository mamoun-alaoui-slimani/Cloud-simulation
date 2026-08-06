#include "CubedAir.h"
#include "constantes.h"
#include <cmath> //pow + exp
#include <iostream>
using namespace Physique;

/**
 * @brief Permet de donner au CubedAir son altitude
 * @param z_ altitude du CubedAir 
 */ 

void CubedAir::setz(double z_){
    z = z_;
}

/**
 * @brief La méthode initialise permet de d'initialiser tous les attributs privés du CubedAir (par défaut tous à 0 quand construit) avec les conditions initiales données dans le complément mathématique.
 */ 
void CubedAir::initialise(bool n){
    evolue();
    sous_la_montagne = n;
}

void CubedAir::evolue(){
    enthalpie = cte - g * z - norme2Vitesse()/2.0;
    temperature = (2.0/7)*M_air_sec*enthalpie/R;
    tau = tau_infini;
    pression = P_infini*std::pow(T_infini, -7/2.0)*std::pow(temperature, 7/2.0);
    P_eau = (tau*pression)/((M_eau/M_air_sec) + tau);
    P_rosee = P_ref*std::exp(13.96-(5208/temperature));
}


/**
 * @brief Setter pour la vitesse du CubedAir
 * @param v std::array<double,3>, obtenue par la méthode ChampPotentiels::vitesse(i, j, k) où i,j,k sont les indices du CubedAir.
 */ 
void CubedAir::setVitesse(std::array<double, 3> const& v){
    vitesse = v;
}

/**
 * @brief Determine si le CubedAir est une cellule nuageuse ou non
 * @return true (1) si cellule nuageuse, false (0) sinon
 */ 
bool CubedAir::formationNuage() const {
    return (P_eau > P_rosee);
}

/**
 * @brief Calcule la norme au carré du vecteur de vitesse du CubedAir.
 */ 
double CubedAir::norme2Vitesse() const{
    return (vitesse[0]*vitesse[0] + vitesse[1]*vitesse[1] + vitesse[2]*vitesse[2]);
}

/**
 * @brief Permet l'affichage de toutes les informations relatives à l'instance courante selon le format
 * V2 h T P Peau Prosee Nuage
 */ 
void CubedAir::afficheNuage() const{
    std::cout << norme2Vitesse() << " " << enthalpie  << " " << temperature << " " << pression << " " <<P_eau << " " << P_rosee << " " << formationNuage();
}

std::ostream& CubedAir::afficheNuage(std::ostream& sortie) const{
    sortie << norme2Vitesse() << " " << enthalpie  << " " << temperature << " " << pression << " " <<P_eau << " " << P_rosee << " " << formationNuage();
    return sortie;
}
/** Pour le moment pas nécessaire
double CubedAir::getEnthalpie() const{
    return enthalpie;
}

double CubedAir::getTemperature() const{
    return temperature;
}

double CubedAir::getPression() const{
    return pression;
}

double CubedAir::getP_eau() const{
    return P_eau;
}

double CubedAir::getP_rosee() const{
    return P_rosee;
}
*/