#pragma once
#include "Montagne.h"


class Gaussienne: public Montagne{
public:
    Gaussienne(double i0, double j0, double h, double sx, double sy);
    virtual double altitude(double i, double j) const override;
    virtual std::ostream& affiche(std::ostream& sortie) const override;
    virtual void dessine_sur(SupportADessin& support) override;
};