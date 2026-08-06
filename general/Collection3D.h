#pragma once
#include <vector>
#include <array>

template<typename type> 
class Collection3D {
    protected:
    //Réunis les attributs communs de Ciel et ChampPotentiels
    std::array<int, 3> nbr_cubes;
    std::array<double, 3> longueurs;
    double taille_cube;
    std::vector<std::vector<std::vector<type> > > collection3D;

    /**
     * @brief Permet d'initialiser les attributs de Collection3D des sous-classes.
     * En protected pour pouvoir être utilisée par les sous-classes, mais en empêchant l'instanciation d'un objet strictement Collection3D
     * 
     * @param Nx Nombres de cubes dans la direction x
     * @param Ny Nombres de cubes dans la direction y
     * @param Nz Nombres de cubes dans la direction z
     */ 
    Collection3D(int Nx, int Ny, int Nz, double lambda)
    : nbr_cubes({Nx, Ny, Nz}), longueurs({lambda * (Nx - 1), lambda * (Ny - 1), lambda * (Nz - 1)}), taille_cube(lambda){
    std::vector<type> vec1D(nbr_cubes[0]); 
    std::vector<std::vector<type> > vec2D(nbr_cubes[1], vec1D);       
    std::vector<std::vector<std::vector<type>>> vec3D(nbr_cubes[2], vec2D);
    collection3D = vec3D; //évite de faire une boucle for Nx fois
    }

    Collection3D(){
        std::vector<type> vec1D(0); 
        std::vector<std::vector<type> > vec2D(1, vec1D);       
        std::vector<std::vector<std::vector<type>>> vec3D(1, vec2D);
        collection3D = vec3D;
    }


};