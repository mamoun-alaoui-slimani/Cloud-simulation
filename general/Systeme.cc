#include "Systeme.h"
#include <iostream>
#include <vector>


Systeme::Systeme(ChampPotentiels const& cp, ChaineDeMontagnes& chaine):
champ(cp), ciel(), chaines(chaine.getChaine())
{}

std::ostream& Systeme::affiche(std::ostream& sortie) const{
    sortie << chaines << std::endl;
    sortie << champ << std::endl;
    ciel.afficheNuages(sortie);
    return sortie;  
}

std::ostream& operator<<(std::ostream& sortie, Systeme const& systeme){
   return systeme.affiche(sortie);
}

void Systeme::dessine_sur(SupportADessin& support){
    ciel.dessine_sur(support);
    chaines.dessine_sur(support);
}

void Systeme::demarre(SupportADessin& support, double seuil, int IteMax, bool affiche){
    champ.initialise(20.0, chaines);
    champ.calcule_laplaciens();
    champ.resolution(seuil, IteMax, affiche);
    ciel = Ciel(champ);
    ciel.evolue();
    dessine_sur(support);
    evolue(support);
}

void Systeme::evolue(SupportADessin& support){
    ciel.evolue();
    dessine_sur(support);
}
