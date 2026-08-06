#pragma once
#include <ostream>
#include <array>

class CubedAir{
    //Attributs
    private:
    double z;
    std::array<double, 3> vitesse;
    double enthalpie;
    double temperature;
    double pression; 
    double P_eau; //Pression partielle de vapeur d'eau
    double P_rosee; //Pression de vapeur saturante
    double tau; //taux d'humidité
    bool sous_la_montagne;
    


    public:
    bool formationNuage() const;
    double norme2Vitesse() const;
    bool sousMontagne() const {return sous_la_montagne;}

    //Initialisation et setters
    void initialise(bool n);
    void evolue();
    void setz(double z_); 
    void setVitesse(std::array<double, 3> const& v);

    //Affichage
    void afficheNuage() const;
    std::ostream& afficheNuage(std::ostream& sortie) const;
    
    /**
    double getEnthalpie() const;
    double getTemperature() const;
    double getPression() const;
    double getP_eau() const;
    double getP_rosee() const;
    */
};