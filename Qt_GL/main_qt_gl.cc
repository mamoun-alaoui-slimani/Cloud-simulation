#include <QApplication>
#include "glwidget.h"
#include "Systeme.h"
#include "Gaussienne.h"
#include <iostream>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  ChampPotentiels champPotentiel(30, 30, 30, 20.0/29.0);


  std::vector<std::unique_ptr<Montagne>> chaine;
  chaine.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(15.0, 15.0, 15.0, 5.0, 5.0)));
  ChaineDeMontagnes chaines(chaine);


/**
  std::vector<std::unique_ptr<Montagne>> Chaine1;
      Chaine1.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(5,5,18,5,10)));

      //création de la sous-chaîne de montagne contenue dans la Chaîne principale
      std::vector<std::unique_ptr<Montagne>> Chaine2;
      Chaine2.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(2,22,12,12,3)));
      Chaine2.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(20,2,15,8,4)));

      //Mise ensemble de la montagne et la sous-chaîne dans la Chaîne.
      Chaine1.emplace_back(std::unique_ptr<Montagne>(new ChaineDeMontagnes(Chaine2)));

      //Initialisation de la Chaîne
      ChaineDeMontagnes chaines(Chaine1);
**/


  Systeme systeme(champPotentiel, chaines);

  GLWidget w(systeme);

  w.show();

  return a.exec();
}
