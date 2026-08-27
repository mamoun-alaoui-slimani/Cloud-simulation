
#include "Sky.h"
#include "PotentialField.h"
#include <cmath>

/**
 * @brief Builds a sky of the given size, with a uniform wind along x.
 * @param Lx box length along x
 * @param Ly box length along y
 * @param Lz box length along z
 * @param lambda sampling step, the edge length of a cell
 * @param velocity first component of the wind velocity
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
 * @brief Updates every air cell of the sky.
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
 * @brief Builds a sky from an already solved potential field.
 * @param cp the solved potential field
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
 * @brief Draws the sky on a renderer.
 * 
 * @param renderer surface to draw on (console, screen, file...)
 */
void Sky::drawOn(Renderer& renderer){
    renderer.draw(*this);
}

/**
 * @brief Whether the air cell at (i, j, k) is cloudy.
 * @param i, j, k indices of the cell in the grid
 * @return true when water vapour has condensed there
 */
bool Sky::isCloudy(int i, int j, int k) const{
    return cells[i][j][k].isCloudy();
}

/**
 * @brief Whether the cell at (i, j, k) sits below the terrain.
 * @param i, j, k indices of the cell in the grid
 * @return true when the cell is buried in the mountain, so not drawn
 */
bool Sky::isBelowTerrain(int i, int j, int k) const{
    return cells[i][j][k].isBelowTerrain();
}

/**
 * @brief Prints the cloud state of every cell.
 * 
 * @param out output stream
 * @return the stream
 */
std::ostream& Sky::printClouds(std::ostream& out) const{
    out <<"Clouds in the sky: " <<std::endl;
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