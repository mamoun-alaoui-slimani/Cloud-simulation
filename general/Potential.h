
#pragma once
#include "Vector2D.h"
#include "array"

class Potential{
    private:
    Vector2D potential;
    Vector2D laplacian;

    public:
    //Constructeurs
    Potential() = default; //needed so a PotentialField can be sized in one go
    Potential(Vector2D const& p, Vector2D const& l) 
    : potential(p), laplacian(l) {}

    //Solving the equations, and velocity
    void computeLaplacian(Potential const& Vi_m1, Potential const& Vj_m1, Potential const& Vk_m1,
    Potential const& Vi_p1, Potential const& Vj_p1, Potential const& Vk_p1);

    void iterate(const double eps);
    void velocity(std::array<double, 3>& velocity, Potential const& Pi_m1, Potential const& Pj_m1, Potential const& Pk_m1,
    Potential const& Pi_p1, Potential const& Pj_p1, Potential const& Pk_p ) const;

    //Queries on the attributes
    double laplacianSquaredNorm() const; //squared norm of the Laplacian
    bool isZero() const; //true when the potential is (0,0), i.e. below the terrain
    
    //Affichage
    void printPotential() const;
    std::ostream& printPotential(std::ostream& out) const;
    void printLaplacian() const;
};