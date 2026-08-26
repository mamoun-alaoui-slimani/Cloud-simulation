#include <QApplication>
#include "glwidget.h"
#include "Systeme.h"
#include "Gaussienne.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  ChampPotentiels champPotentiel(30, 30, 30, 20.0/29.0);

  /* Une chaîne peut contenir d'autres chaînes : voir les tests, qui
     vérifient qu'une chaîne imbriquée se comporte comme une montagne. */
  std::vector<std::unique_ptr<Montagne>> chaine;
  chaine.emplace_back(std::unique_ptr<Montagne>(new Gaussienne(15.0, 15.0, 15.0, 5.0, 5.0)));
  ChaineDeMontagnes chaines(chaine);

  Systeme systeme(champPotentiel, chaines);

  GLWidget w(systeme);
  w.show();

  return a.exec();
}
