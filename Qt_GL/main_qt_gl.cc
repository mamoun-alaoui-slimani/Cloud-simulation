#include <QApplication>
#include "GLWidget.h"
#include "System.h"
#include "GaussianPeak.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  PotentialField potentialField(30, 30, 30, 20.0/29.0);

  /* Une chaîne peut contenir d'autres chaînes : voir les tests, qui
     vérifient qu'une chaîne imbriquée se comporte comme une mountain. */
  std::vector<std::unique_ptr<Mountain>> chain;
  chain.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(15.0, 15.0, 15.0, 5.0, 5.0)));
  MountainChain terrain(chain);

  System system(potentialField, terrain);

  GLWidget w(system);
  w.show();

  return a.exec();
}
