#include "Montagne.h"



Montagne::Montagne(double i0, double j0, double h, double sx, double sy)
    : i0(i0), j0(j0), hauteur(h), sigma_x(sx), sigma_y(sy)
{ }

std::ostream& Montagne::affiche(std::ostream& sortie) const {
    sortie << "i0: " << i0 << ", j0: " << j0 << ", hauteur: " << hauteur << ", sigma_x: " << sigma_x << ", sigma_y: " << sigma_y << std::endl;
    return sortie;
}

std::ostream& operator<<(std::ostream& sortie, Montagne const& m){
    return m.affiche(sortie);
}