#include "ChaineDeMontagnes.h"
#include "Montagne.h"
#include "SupportADessin.h"
#include <vector>
#include <memory>

/**
 * @brief Construit une nouvelle chaine de Montagne
 * 
 * @param v 
 */
ChaineDeMontagnes::ChaineDeMontagnes(std::vector<std::unique_ptr<Montagne>>& v){
//std::move permet de déplacer l'objet sans le copier
//pas de const sinon pas de std::move possible.    
    for(auto& montagne : v){  
        chaine.emplace_back(std::move(montagne));
    }
}

/**
 * @brief Dessine la Chaine de Montagnes sur un support
 * 
 * @param support support sur lequel dessiner (Texte, écran...)
 */
void ChaineDeMontagnes::dessine_sur(SupportADessin& support){
    support.dessine(*this);
}

/**
 * @brief Calcule et renvoie l'altitude de la montagne au point (i,j)
 * 
 * @param i abscisse
 * @param j ordonnée
 * @return double altitude de la montagne au point (i,j)
 */
double ChaineDeMontagnes::altitude(double i, double j) const {
    double max(0.0);
    double tmp(0.0);
    for(const std::unique_ptr<Montagne>& u : chaine){
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
 * @return std::vector<std::unique_ptr<Montagne>>& 
 */
std::vector<std::unique_ptr<Montagne>>& ChaineDeMontagnes::getChaine() {
    return chaine;
}

/**
 * @brief Surcharge de l'opérateur d'affichage
 * 
 * @param sortie flot de sortie
 * @param chaine Chaine de montagne à afficher
 * @return le flot de sortie modifié
 */
std::ostream& operator<<(std::ostream& sortie, ChaineDeMontagnes const& chaine){
    return chaine.affiche(sortie);
}

/**
 * @brief Affiche la chaine de montagne sur un flot de sortie
 * 
 * @param sortie flot de sortie
 * @return flot de sortie modifié
 */
std::ostream& ChaineDeMontagnes::affiche(std::ostream& sortie) const{
    sortie <<"Une chaîne de montagne: " <<std::endl;
    for(const auto& montagne : chaine){
        sortie << *montagne;
        sortie << std::endl;
    }
    return sortie;
}
