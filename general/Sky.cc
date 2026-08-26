
#include "Sky.h"
#include "PotentialField.h"
#include <cmath>

/**
 * @brief Constructeur de Sky demandé. Initialise tous les cubes d'air selon les conditions initiales et donne une velocity du vent uniforme selon une direction (x).
 * @param Lx longueur de la boîte dans la direction x
 * @param Ly longueur de la boîte dans la direction y
 * @param Lz longueur de la boîte dans la direction z
 * @param lambda pas d'échantillonage
 * @param velocity première composante de la velocity des cubes d'air.
 */ 
Sky::Sky(double Lx, double Ly, double Lz, double lambda, double velocity):
Grid3D<AirCell>::Grid3D(ceil(1 + (Lx/lambda)), ceil(1 + (Ly/lambda)), ceil(1 + (Lz/lambda)), lambda){
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                cells[i][j][k].setZ(cellSize * k);
                cells[i][j][k].setVelocity(std::array<double, 3>({velocity, 0.0, 0.0}));
            }
        }
    }
}

/**
 * @brief Met à jour les valeurs contenues dans les AirCell du Sky
 * 
 */
void Sky::update(){
    for(std::vector<std::vector<AirCell>>& vec2D : cells){
        for(std::vector<AirCell>& vec1D : vec2D){
            for(AirCell& cube : vec1D){
                cube.update();
            }
        }
    }
}

Sky::Sky() : Grid3D<AirCell>::Grid3D<AirCell>() {}

/**
 * @brief Constructeur de sky se basant sur une instance déjà créée de PotentialField. Initialise aussi les attributs avec les conditions initiales.
 * @param cp Instance de ChampPotentiel
 */ 
Sky::Sky(PotentialField const& cp)
: Grid3D::Grid3D(cp.getCellCount()[0], cp.getCellCount()[1], cp.getCellCount()[2], cp.getCellSize()) {
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                cells[i][j][k].setZ(cellSize * k);
                cells[i][j][k].setVelocity(cp.velocity(i,j,k));
                cells[i][j][k].initialize(cp.isPotentialZero(i,j,k));
            }
        }
    }
}

/**
 * @brief Dessine le Sky sur un renderer
 * 
 * @param renderer renderer sur lequel dessiner le Sky (Texte, graphique, écran, fichier...)
 */
void Sky::drawOn(Renderer& renderer){
    renderer.draw(*this);
}

/**
 * @brief Indique si le cube d'air en (i, j, k) est nuageux
 * @param i, j, k indices du cube dans la grille
 * @return true si la vapeur d'eau y a condensé
 */
bool Sky::isCloudy(int i, int j, int k) const{
    return cells[i][j][k].isCloudy();
}

/**
 * @brief Indique si le cube d'air en (i, j, k) est situé sous le relief
 * @param i, j, k indices du cube dans la grille
 * @return true si le cube est enterré dans la mountain (donc pas à dessiner)
 */
bool Sky::isBelowTerrain(int i, int j, int k) const{
    return cells[i][j][k].isBelowTerrain();
}

/**
 * @brief Affiche les Nuages sur un stream de out
 * 
 * @param out stream de out
 * @param m Mountain à utiliser pour évaluer la formation de Nuage
 * @return stream de out modifié
 */
std::ostream& Sky::printClouds(std::ostream& out) const{ //ne devrait pas prendre la Mountain en paramètre car pas accès lors de l'affichage avec TextRenderer
    out <<"Les nuages du ciel : " <<std::endl;
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                if(not(i == 0 or j == 0 or i == cellCount[0] - 1 or j == cellCount[1] - 1 or k == 0 or k == cellCount[2]-1)){
                    out << i << " " << j <<" " << k <<" ";
                    cells[i][j][k].printState(out);
                    out << " " <<std::endl;
                }
            }
        }
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Sky const& sky){
    return sky.printClouds(out);
}