#include <QApplication>
#include "GLWidget.h"
#include "System.h"
#include "GaussianPeak.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    PotentialField potentialField(30, 30, 30, 20.0 / 29.0);

    /* A chain can contain other chains: the tests check that a nested
       chain behaves exactly like a single mountain. */
    std::vector<std::unique_ptr<Mountain>> chain;
    chain.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(15.0, 15.0, 15.0, 5.0, 5.0)));
    MountainChain terrain(chain);

    System system(potentialField, terrain);

    GLWidget w(system);
    w.show();

    return a.exec();
}
