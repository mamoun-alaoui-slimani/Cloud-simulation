#include "MountainChain.h"
#include "Mountain.h"
#include "Renderer.h"
#include <vector>
#include <memory>

/**
 * @brief Builds a chain that takes ownership of the given mountains.
 * 
 * @param v mountains to move into the chain
 */
MountainChain::MountainChain(std::vector<std::unique_ptr<Mountain>>& v){
//std::move transfers ownership without copying;
//the parameter cannot be const, or the move would be impossible.
    for(auto& mountain : v){  
        chain.emplace_back(std::move(mountain));
    }
}

/**
 * @brief Draws the chain on a renderer.
 * 
 * @param renderer surface to draw on (console, screen...)
 */
void MountainChain::drawOn(Renderer& renderer){
    renderer.draw(*this);
}

/**
 * @brief Altitude of the chain at (i, j): the highest of its components.
 * 
 * @param i abscisse
 * @param j ordinate
 * @return the altitude at (i, j)
 */
double MountainChain::altitude(double i, double j) const {
    double max(0.0);
    double tmp(0.0);
    for(const std::unique_ptr<Mountain>& u : chain){
        tmp = u->altitude(i, j);
        if(tmp > max){
            max = tmp;
        }
    }
    return max;
}

/**
 * @brief The mountains this chain owns.
 */
std::vector<std::unique_ptr<Mountain>>& MountainChain::getChain() {
    return chain;
}

/**
 * @brief Stream insertion operator.
 * 
 * @param out output stream
 * @param chain chain to print
 * @return the stream
 */
std::ostream& operator<<(std::ostream& out, MountainChain const& chain){
    return chain.print(out);
}

/**
 * @brief Prints the chain and every mountain it contains.
 * 
 * @param out output stream
 * @return the stream
 */
std::ostream& MountainChain::print(std::ostream& out) const{
    out <<"A mountain chain: " <<std::endl;
    for(const auto& mountain : chain){
        out << *mountain;
        out << std::endl;
    }
    return out;
}
