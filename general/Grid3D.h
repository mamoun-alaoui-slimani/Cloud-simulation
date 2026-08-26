#pragma once
#include <vector>
#include <array>

template<typename type> 
class Grid3D {
    protected:
    //Réunis les attributs communs de Sky et PotentialField
    std::array<int, 3> cellCount;
    std::array<double, 3> lengths;
    double cellSize;
    std::vector<std::vector<std::vector<type> > > cells;

    /**
     * @brief Permet d'initialiser les attributs de Grid3D des sous-classes.
     * En protected pour pouvoir être utilisée par les sous-classes, mais en empêchant l'instanciation d'un objet strictement Grid3D
     * 
     * @param Nx Nombres de cubes dans la direction x
     * @param Ny Nombres de cubes dans la direction y
     * @param Nz Nombres de cubes dans la direction z
     */ 
    Grid3D(int Nx, int Ny, int Nz, double lambda)
    : cellCount({Nx, Ny, Nz}), lengths({lambda * (Nx - 1), lambda * (Ny - 1), lambda * (Nz - 1)}), cellSize(lambda){
    std::vector<type> vec1D(cellCount[0]); 
    std::vector<std::vector<type> > vec2D(cellCount[1], vec1D);       
    std::vector<std::vector<std::vector<type>>> vec3D(cellCount[2], vec2D);
    cells = vec3D; //évite de faire une boucle for Nx fois
    }

    Grid3D(){
        std::vector<type> vec1D(0); 
        std::vector<std::vector<type> > vec2D(1, vec1D);       
        std::vector<std::vector<std::vector<type>>> vec3D(1, vec2D);
        cells = vec3D;
    }


};