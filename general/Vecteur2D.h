
#pragma once 
#include <iostream>

/**
 * @brief Représentation des vecteurs de dimension 2 au sens mathématique
 * 
 */
class Vecteur2D{

public:

//Propres à l'instance courante
    Vecteur2D(double x = 0.0, double y = 0.0); //valeurs par défaut pour constructeur par défaut.
    void set_coord(double NVx, double NVy);
    std::ostream& affiche(std::ostream& sortie) const;
//Getters
    double getX() const {return x_;}
    double getY() const {return y_;}
//Opérations sur les vecteurs 
    bool compare(Vecteur2D const& ACompare, double precis = 1e-10) const; 
    double norme2() const;
    double norme() const;

    Vecteur2D& operator+=(Vecteur2D const& Add);
    Vecteur2D& operator-=(Vecteur2D const& Sous);
    Vecteur2D& operator*=(double scalaire);
     
    double operator*=(Vecteur2D const& VecScal);

//Déclaration des attributs, voir REPONSES P1.1 pour plus de détails
private:

    double x_;
    double y_;
};

//pas d'utilisation de namespace std dans les headers.
std::ostream& operator<<(std::ostream& sortie, Vecteur2D const& vec);

bool operator==(Vecteur2D const& vec1, Vecteur2D const& vec2);
bool operator!=(Vecteur2D const& vec1, Vecteur2D const& vec2);

Vecteur2D operator+(Vecteur2D const& vec1, Vecteur2D vec2);
Vecteur2D operator-(Vecteur2D vec1, Vecteur2D const& vec2);
double operator*(Vecteur2D vec1, Vecteur2D const& vec2);
Vecteur2D operator*(Vecteur2D vec, double scalaire);
//commutativité de la loi de multiplication externe
Vecteur2D operator*(double scalaire, Vecteur2D);
//retourne vecteur unitaire
Vecteur2D operator~(Vecteur2D const& vec);
//Remplace oppose()
Vecteur2D operator-(Vecteur2D const& vec);
