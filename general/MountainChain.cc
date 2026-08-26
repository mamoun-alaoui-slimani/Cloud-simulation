#include "MountainChain.h"
#include "Mountain.h"
#include "Renderer.h"
#include <vector>
#include <memory>

/**
 * @brief Construit une nouvelle chain de Mountain
 * 
 * @param v 
 */
MountainChain::MountainChain(std::vector<std::unique_ptr<Mountain>>& v){
//std::move permet de déplacer l'objet sans le copier
//pas de const sinon pas de std::move possible.    
    for(auto& mountain : v){  
        chain.emplace_back(std::move(mountain));
    }
}

/**
 * @brief Dessine la Chaine de Montagnes sur un renderer
 * 
 * @param renderer renderer sur lequel dessiner (Texte, écran...)
 */
void MountainChain::drawOn(Renderer& renderer){
    renderer.draw(*this);
}

/**
 * @brief Calcule et renvoie l'altitude de la mountain au point (i,j)
 * 
 * @param i abscisse
 * @param j ordonnée
 * @return double altitude de la mountain au point (i,j)
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
 * @brief Renvoie le vecteur de pointeurs sur les montagnes de la Chaine de Montagnes
 * 
 * @return std::vector<std::unique_ptr<Mountain>>& 
 */
std::vector<std::unique_ptr<Mountain>>& MountainChain::getChain() {
    return chain;
}

/**
 * @brief Surcharge de l'opérateur d'affichage
 * 
 * @param out stream de out
 * @param chain Chaine de mountain à afficher
 * @return le stream de out modifié
 */
std::ostream& operator<<(std::ostream& out, MountainChain const& chain){
    return chain.print(out);
}

/**
 * @brief Affiche la chain de mountain sur un stream de out
 * 
 * @param out stream de out
 * @return stream de out modifié
 */
std::ostream& MountainChain::print(std::ostream& out) const{
    out <<"Une chaîne de montagne: " <<std::endl;
    for(const auto& mountain : chain){
        out << *mountain;
        out << std::endl;
    }
    return out;
}
