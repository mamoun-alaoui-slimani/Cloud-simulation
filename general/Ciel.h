#pragma once
#include <vector>
#include <array>
#include "CubedAir.h"
#include "Collection3D.h"
#include "ChampPotentiels.h"
#include "ChaineDeMontagnes.h"
#include "Dessinable.h"
#include "SupportADessin.h"

class Ciel : Collection3D<CubedAir>, Dessinable {
    private:
    void initialiserVitesses();

    public:
    Ciel(ChampPotentiels const& cp);
    Ciel(double Lx, double Ly, double Lz, double lambda, double vitesse);
    Ciel();
    void dessine_sur(SupportADessin& support);
    std::array<int, 3> getCubes() const { return nbr_cubes; }
    bool Nuageux(int i, int j, int k) const;
    bool SousMontagne(int i, int j, int k) const;
    void evolue();

    //void afficheNuages() const; A décommenter une fois la solution trouvée
    std::ostream& afficheNuages(std::ostream& sortie) const;
};

std::ostream& operator<<(std::ostream& sortie, Ciel const& ciel);
