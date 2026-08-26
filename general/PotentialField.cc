
#include <array>
#include <iostream>
#include <iomanip> 
#include "PotentialField.h"

/**
 * @brief Construit un nouvel objet ChampsPotentiels. 
 * Appelle le constructeur de Grid3D.
 * 
 * @param Nx nombres de cubes désirés sur l'axe des x
 * @param Ny nombres de cubes désirés sur l'axe des y
 * @param Nz nombres de cubes désirés sur l'axe des z
 * @param lambda Taille des cubes
 */
PotentialField::PotentialField(int Nx, int Ny, int Nz, double lambda)
:Grid3D<Potential>::Grid3D(Nx, Ny, Nz, lambda) {} 

/**
 * @brief Initialise le champs de potentiels.
 * Les potentiels sont intialisés à (0,0) si sous la mountain.
 * Les laplaciens sont tous initialisés à (0,0).
 * 
 * @param v velocity aux bords
 * @param m Mountain à utiliser pour initialiser le champs de potentiels
 */
void PotentialField::initialize(double v, const Mountain &m){
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                if(k >= m.altitude(i, j) or i == 0 or j == 0 or i == cellCount[0] - 1 or j == cellCount[1] - 1) {
                    cells[i][j][k] = Potential(Vector2D(-v*(k*cellSize)/2, v*(j*cellSize - (lengths[1]/2))/2), Vector2D());
                } else {
                    cells[i][j][k] = Potential(); 
                    //Normalement pas nécessaire puisque notre ChampPotentiel est construit avec les deux vecteurs de Potential initialisés à (0,0) mais pour le moment laisser au cas où on veuille réinitialiser un ChampPotentiel dans lequel les Potentiels ont été modifiés.
                }
            }
        }
    }
}

/**
 * @brief Calcul les Laplaciens pour chaque Potential.
 * 
 * Le calcul ne se fait pas si le point est sous la mountain.
 */
void PotentialField::computeLaplacians(){
    for(int i(1); i < cellCount[0] - 1; ++i){
        for(int j(1); j < cellCount[1] - 1; ++j){
            for(int k(1); k < cellCount[2] - 1; ++k){
                //Pour vérifier si sous la mountain ou non
                if(not cells[i][j][k].isZero()){
                cells[i][j][k].computeLaplacian(cells[i-1][j][k], cells[i][j-1][k], cells[i][j][k-1],
                cells[i+1][j][k], cells[i][j+1][k], cells[i][j][k+1]);
                }
            }
        }
    }
}

/**
 * @brief Affiche les potentiels du champs selon le format:
 * i j k P1 P2
 */
void PotentialField::printPotentials() const {
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                std::cout << i  <<" " << j <<" " << k <<" ";
                cells[i][j][k].printPotential();
                std::cout << std::endl;
            }
        }
    }
}

/**
 * @brief Affiche les Laplaciens du champs selon le format:
 *  i j k L2 L3
 */
void PotentialField::printLaplacians() const {
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                std::cout << i << " " << j <<" " << k <<" ";
                cells[i][j][k].printLaplacian();
                std::cout << " " <<std::endl;
            }
        }
    }
}
/**
 * @brief calcule l'residual nécessaire pour déterminer si convergence.
 * @return double: Somme de la norm au carré de tous les laplaciens.
 */ 
double PotentialField::residual() const {
    double squaredNorm(0.0);
    for(int i(0); i < cellCount[0]; i++){
        for(int j(0); j < cellCount[1]; j++){
            for(int k(0); k < cellCount[2]; k++){
                squaredNorm += cells[i][j][k].laplacianSquaredNorm();
            }
        }
    }
    return squaredNorm;
}

/**
 * @brief Appelle la méthode Potential::iterate() pour tous les points intérieurs de la boîte.
 */ 
void PotentialField::iterate(const double eps){
    for(int i(1); i < cellCount[0] -1; i++){
        for(int j(1); j < cellCount[1] -1; j++){
            for(int k(1); k < cellCount[2] -1; k++){
                cells[i][j][k].iterate(eps);
            }
        }
    }
}

/**
 * @brief Résoud les équations de Laplace. Tant que l'residual obtenue est plus grande que le threshold ou que le nombre d'itération est plus petit que le nombre max: apppelle la méthode PotentialField::iterate(). Puis recalcule les laplaciens avec les nouvelles valeurs des potentiels.
 * 
 * @param threshold threshold d'residual pour déterminer si convergence ou non
 * @param maxIterations nombre max d'itération pour éviter boucle infini
 * @param print si vrai la méthode print les potentiels et laplaciens à chaque nouvelle itération
 */ 
void PotentialField::solve(double threshold, int maxIterations, bool print){
    int iterationCount(1);
    double residual(this->residual());
    while((threshold < residual) and (iterationCount <= maxIterations)){
        this->iterate();
        this->computeLaplacians();
        if(print){
            this->printPotentials(); //Si verbeuse
            this->printLaplacians();
        }
        residual = this->residual(); //Actualisation de l'residual avec les nouvelles valeurs des potentiels.
        iterationCount++;
    }
}

/**
 * @brief Pour les indices donnés en paramètre, calcule la velocity du vent associée au point. La velocity au bord n'étant pas définie (sinon segmentation fault), on la met à (0,0,0).
 * @param i 
 * @param j 
 * @param k
 * 
 * @return Retourne un vecteur 3D où les composantes sont la velocity vx, vy et vz respectivement.
 */ 
std::array<double, 3> PotentialField::velocity(int i, int j, int k) const{
    std::array<double, 3> velocity({0,0,0}); //Array de 3 double nul

    if(i == 0 or j == 0 or k == 0 or i == cellCount[0] - 1 or j == cellCount[1] -1 or k == cellCount[2] - 1) return velocity;

    cells[i][j][k].velocity(velocity, cells[i-1][j][k], cells[i][j-1][k], cells[i][j][k-1], cells[i+1][j][k], cells[i][j+1][k], cells[i][j][k+1]);

    for(size_t i(0); i < velocity.size(); ++i){
        velocity[i] *= (1 / (2 * cellSize));
    }
    return velocity;
}

/**
 * @brief Indique si le potential en (i, j, k) est nul, donc sous le relief.
 *
 * Les indices hors de la boite sont consideres comme nuls plutot que
 * d'indexer hors bornes.
 */
bool PotentialField::isPotentialZero(int i, int j, int k) const {
    if(0 <= i and i < cellCount[0] and 0 <= j and j < cellCount[1] and 0 <= k and k < cellCount[2]){
        return cells[i][j][k].isZero();
    }
    return true;
}

/** @brief Affiche les indices du point, les composantes du vecteur potential, la velocity et sa norm au carré (v2) pour tous les points de notre boîte selon le format suivant:
 * i j k P1 P2 vx vy vz v2 
 */ 
void PotentialField::printVelocities() const{
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                if(not cells[i][j][k].isZero()){

                    std::cout << i << " " << j <<" " << k <<" ";
                    cells[i][j][k].printPotential();

                    std::array<double, 3> vit(this->velocity(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    std::cout <<std::setprecision(6) << " " <<vit[0] <<" " <<vit[1] <<" " <<vit[2] <<" " <<V2 <<" " <<std::endl;
                }
            }
        }
    }
}

/**
 * @brief Affiche les vitesses sur un stream de out
 * 
 * @param out stream de out
 * @return stream de out modifié
 */
std::ostream& PotentialField::printVelocities(std::ostream& out) const{
    for(int i(0); i < cellCount[0]; ++i){
        for(int j(0); j < cellCount[1]; ++j){
            for(int k(0); k < cellCount[2]; ++k){
                if(not cells[i][j][k].isZero()){

                    out << i << " " << j <<" " << k <<" ";
                    cells[i][j][k].printPotential(out);

                    std::array<double, 3> vit(this->velocity(i, j, k));
                    double V2(vit[0] * vit[0] + vit[1] * vit[1] + vit[2] * vit[2]);

                    out <<std::setprecision(6) << " " <<vit[0] <<" " <<vit[1] <<" " <<vit[2] <<" " <<V2 <<" " <<std::endl;
                }
            }
        }
    }
    return out;
}

/**
 * @brief Surcharge de l'opérateur d'affichage
 * 
 * @param out stream de out
 * @param field Champs de Potentiels à afficher
 * @return stream de out modifié
 */
std::ostream& operator<<(std::ostream& out, PotentialField const& field){
    out <<"Le système est constitué d'un champ de Potentiel:  " <<std::endl;
    return field.printVelocities(out);
}

/**
 * @brief Getter pour l'attribut cellSize
 */ 
double PotentialField::getCellSize() const {
    return cellSize;
}

/**
 * @brief Getter pour l'attribut cellCount
 */ 
std::array<int, 3> PotentialField::getCellCount() const {
    return cellCount;
}
