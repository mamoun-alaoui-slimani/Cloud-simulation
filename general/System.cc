#include "System.h"
#include <iostream>
#include <vector>


System::System(PotentialField const& cp, MountainChain& chain):
field(cp), sky(), terrain(chain.getChain())
{}

std::ostream& System::print(std::ostream& out) const{
    out << terrain << std::endl;
    out << field << std::endl;
    sky.printClouds(out);
    return out;  
}

std::ostream& operator<<(std::ostream& out, System const& system){
   return system.print(out);
}

void System::drawOn(Renderer& renderer){
    sky.drawOn(renderer);
    terrain.drawOn(renderer);
}

void System::start(Renderer& renderer, double threshold, int maxIterations, bool print){
    field.initialize(20.0, terrain);
    field.computeLaplacians();
    field.solve(threshold, maxIterations, print);
    sky = Sky(field);
    sky.update();
    drawOn(renderer);
    update(renderer);
}

void System::update(Renderer& renderer){
    sky.update();
    drawOn(renderer);
}
