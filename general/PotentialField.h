
#pragma once
#include <iostream>
#include <array>
#include "Mountain.h"
#include "Grid3D.h"
#include "Potential.h"
#include "MountainChain.h"


/**
 * @brief Représentation d'un Champs de Potentiels
 * 
 */
class PotentialField : protected Grid3D<Potential>{
    public:
    PotentialField(int Nx, int Ny, int Nz, double lambda);
    void initialize(double v, const Mountain &m);
    
    std::array<double, 3> velocity(int i, int j, int k) const;
    
    //Résolution
    void computeLaplacians();
    void solve(double threshold = 1e-4, int maxIterations = 5000, bool print = false);

    //AFFICHAGE
    void printPotentials() const;
    void printLaplacians() const;
    void printVelocities() const;
    std::ostream& printVelocities(std::ostream& out) const;

    //getters
    double getCellSize() const;
    std::array<int, 3> getCellCount() const;
    bool isPotentialZero(int i, int j, int k) const;

    private:
    void iterate(const double eps = 0.1);
    double residual() const;
};

std::ostream& operator<<(std::ostream& out, PotentialField const& field);