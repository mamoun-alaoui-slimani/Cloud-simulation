#include "Gaussienne.h"
#include "Montagne.h"
#include "SupportADessin.h"
#include <cmath>

Gaussienne::Gaussienne(double i0, double j0, double h, double sx, double sy):
Montagne(i0, j0, h, sx, sy) {}

double Gaussienne::altitude(double i, double j) const {
    double diff_i = ((-(i-i0)*(i-i0)) / (2.0*sigma_x*sigma_x));
    double diff_j = ((-(j-j0)*(j-j0)) / (2.0*sigma_y*sigma_y));
    double H = (hauteur * std::exp( diff_i + diff_j ));
    if(H < 0.5) H = 0.0;
    return H;
}

void Gaussienne::dessine_sur(SupportADessin& support){
    support.dessine(*this);
}

std::ostream& Gaussienne::affiche(std::ostream& sortie) const {
    sortie <<"Une montagne Gaussienne de paramètres: " <<std::endl;
    return Montagne::affiche(sortie);
}