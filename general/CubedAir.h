#pragma once
#include <ostream>
#include <array>

class CubedAir{
    //Attributs
    private:
    double z = 0.0;
    std::array<double, 3> vitesse = {0.0, 0.0, 0.0};
    double enthalpie = 0.0;
    double temperature = 0.0;
    double pression = 0.0;
    double P_eau = 0.0; //Pression partielle de vapeur d'eau
    double P_rosee = 0.0; //Pression de vapeur saturante
    double tau = 0.0; //taux d'humidité
    bool sous_la_montagne = false;



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
    
    //Accesseurs (utiles a l'affichage et aux tests)
    double getEnthalpie() const {return enthalpie;}
    double getTemperature() const {return temperature;}
    double getPression() const {return pression;}
    double getP_eau() const {return P_eau;}
    double getP_rosee() const {return P_rosee;}
};