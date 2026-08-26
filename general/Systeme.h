#pragma once
#include "Ciel.h"
#include "ChampPotentiels.h"
#include "ChaineDeMontagnes.h"
#include "Dessinable.h"

class Systeme : Dessinable{
private:
    ChampPotentiels champ;
    Ciel ciel;
    ChaineDeMontagnes chaines; //Collection de montagnes

public:
    Systeme(ChampPotentiels const& cp, ChaineDeMontagnes& chaine);
    /* Pas de copie : ChaineDeMontagnes detient un vector<unique_ptr<Montagne>>,
       qui n'est pas copiable sans un clone() virtuel sur Montagne. */
    Systeme(Systeme const&) = delete;
    Systeme& operator=(Systeme const&) = delete;
    std::ostream& affiche(std::ostream& sortie) const;
    void evolue(SupportADessin& support);
    virtual void dessine_sur(SupportADessin& support) override;
    void demarre(SupportADessin& support, double seuil = 1e-4, int IteMax = 5000, bool affiche = false);
};

std::ostream& operator<<(std::ostream& sortie, Systeme const& systeme);
