#pragma once
#include <ostream>
#include <array>

class AirCell{
    //Attributs
    private:
    double z = 0.0;
    std::array<double, 3> velocity = {0.0, 0.0, 0.0};
    double enthalpy = 0.0;
    double temperature = 0.0;
    double pressure = 0.0;
    double vapourPressure = 0.0; //Pression partielle de vapeur d'eau
    double saturationPressure = 0.0; //Pression de vapeur saturante
    double humidity = 0.0; //taux d'humidité
    bool belowTerrain = false;



    public:
    bool isCloudy() const;
    double velocitySquaredNorm() const;
    bool isBelowTerrain() const {return belowTerrain;}

    //Initialisation et setters
    void initialize(bool n);
    void update();
    void setZ(double z_); 
    void setVelocity(std::array<double, 3> const& v);

    //Affichage
    void printState() const;
    std::ostream& printState(std::ostream& out) const;
    
    //Accesseurs (utiles a l'affichage et aux tests)
    double getEnthalpy() const {return enthalpy;}
    double getTemperature() const {return temperature;}
    double getPressure() const {return pressure;}
    double getVapourPressure() const {return vapourPressure;}
    double getSaturationPressure() const {return saturationPressure;}
};