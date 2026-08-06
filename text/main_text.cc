#include <iostream>
#include "TextViewer.h"
#include "Systeme.h"
#include "Gaussienne.h"
using namespace std;

int main()
{
  TextViewer ecran;

  ChampPotentiels champPotentiel(30, 30, 30, 20.0/29.0);
  std::vector<std::unique_ptr<Montagne>> chaine;
  chaine.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(15.0, 15.0, 15.0, 5.0, 5.0)));
  ChaineDeMontagnes chaines(chaine);


  Systeme systeme(champPotentiel, chaines);
  systeme.demarre(ecran);

  return 0;
}
