
#pragma once
#include "Vector2D.h"
#include "array"

class Potential{
    private:
    Vector2D potential;
    Vector2D laplacian;

    public:
    //Constructeurs
    Potential() = default; //Nécessaire pour l'initialisation simple d'une instance de PotentialField
    Potential(Vector2D const& p, Vector2D const& l) 
    : potential(p), laplacian(l) {}

    //Résolution des équations et velocity
    void computeLaplacian(Potential const& Vi_m1, Potential const& Vj_m1, Potential const& Vk_m1,
    Potential const& Vi_p1, Potential const& Vj_p1, Potential const& Vk_p1);

    void iterate(const double eps);
    void velocity(std::array<double, 3>& velocity, Potential const& Pi_m1, Potential const& Pj_m1, Potential const& Pk_m1,
    Potential const& Pi_p1, Potential const& Pj_p1, Potential const& Pk_p ) const;

    //Informations sur attributs
    double laplacianSquaredNorm() const; //retourne la norm au carré du laplacian
    bool isZero() const; //Permet de savoir si le potential est à (0,0)
    
    //Affichage
    void printPotential() const;
    std::ostream& printPotential(std::ostream& out) const;
    void printLaplacian() const;
};