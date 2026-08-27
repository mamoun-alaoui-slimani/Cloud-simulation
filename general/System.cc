#include "System.h"
#include <iostream>
#include <vector>

System::System(PotentialField const& cp, MountainChain& chain)
    : field(cp), sky(), terrain(chain.getChain()) {}

std::ostream& System::print(std::ostream& out) const {
    out << terrain << std::endl;
    out << field << std::endl;
    sky.printClouds(out);
    return out;
}

std::ostream& operator<<(std::ostream& out, System const& system) {
    return system.print(out);
}

void System::drawOn(Renderer& renderer) {
    sky.drawOn(renderer);
    terrain.drawOn(renderer);
}

/**
 * @brief Solves the flow field over the terrain and builds the sky from it.
 *
 * This is the expensive part of the simulation and it touches no renderer,
 * so a caller is free to run it on a worker thread.
 */
void System::solve(double threshold, int maxIterations, bool print) {
    field.initialize(20.0, terrain);
    field.computeLaplacians();
    field.solve(threshold, maxIterations, print);
    sky = Sky(field);
    sky.update();
}

/**
 * @brief Advances the sky by one step.
 */
void System::step() {
    sky.update();
}
