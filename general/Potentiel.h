
#pragma once
#include "Vecteur2D.h"
#include "array"

class Potentiel{
    private:
    Vecteur2D potentiel;
    Vecteur2D laplacien;

    public:
    //Constructeurs
    Potentiel() = default; //Nécessaire pour l'initialisation simple d'une instance de ChampPotentiels
    Potentiel(Vecteur2D const& p, Vecteur2D const& l) 
    : potentiel(p), laplacien(l) {}

    //Résolution des équations et vitesse
    void calcule_laplacien(Potentiel const& Vi_m1, Potentiel const& Vj_m1, Potentiel const& Vk_m1,
    Potentiel const& Vi_p1, Potentiel const& Vj_p1, Potentiel const& Vk_p1);

    void iteration(const double eps);
    void vitesse(std::array<double, 3>& vitesse, Potentiel const& Pi_m1, Potentiel const& Pj_m1, Potentiel const& Pk_m1,
    Potentiel const& Pi_p1, Potentiel const& Pj_p1, Potentiel const& Pk_p ) const;

    //Informations sur attributs
    double Norme2Laplacien() const; //retourne la norme au carré du laplacien
    bool PotentielNul() const; //Permet de savoir si le potentiel est à (0,0)
    
    //Affichage
    void affichePotentiel() const;
    std::ostream& affichePotentiel(std::ostream& sortie) const;
    void afficheLaplacien() const;
};