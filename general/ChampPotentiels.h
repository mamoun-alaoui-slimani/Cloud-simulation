
#pragma once
#include <iostream>
#include <array>
#include "Montagne.h"
#include "Collection3D.h"
#include "Potentiel.h"
#include "ChaineDeMontagnes.h"


/**
 * @brief Représentation d'un Champs de Potentiels
 * 
 */
class ChampPotentiels : protected Collection3D<Potentiel>{
    public:
    ChampPotentiels(int Nx, int Ny, int Nz, double lambda);
    void initialise(double v, const Montagne &m);
    
    std::array<double, 3> vitesse(int i, int j, int k) const;
    
    //Résolution
    void calcule_laplaciens();
    void resolution(double seuil = 1e-4, int IteMax = 5000, bool affiche = false);

    //AFFICHAGE
    void affiche_potentiels() const;
    void affiche_laplaciens() const;
    void affiche_vitesses() const;
    std::ostream& affiche_vitesses(std::ostream& sortie) const;

    //getters
    double getTailleCube() const;
    std::array<int, 3> getNbrCubes() const;
    bool potentielNul(int i, int j, int k) const;

    private:
    void iteration(const double eps = 0.1);
    double erreur() const;
};

std::ostream& operator<<(std::ostream& sortie, ChampPotentiels const& champ);