
#pragma once 
#include <iostream>

/**
 * @brief Représentation des vecteurs de dimension 2 au sens mathématique
 * 
 */
class Vector2D{

public:

//Propres à l'instance courante
    Vector2D(double x = 0.0, double y = 0.0); //valeurs par défaut pour constructeur par défaut.
    void setCoords(double NVx, double NVy);
    std::ostream& print(std::ostream& out) const;
//Getters
    double getX() const {return x_;}
    double getY() const {return y_;}
//Opérations sur les vecteurs 
    bool compare(Vector2D const& other, double precision = 1e-10) const; 
    double squaredNorm() const;
    double norm() const;

    Vector2D& operator+=(Vector2D const& other);
    Vector2D& operator-=(Vector2D const& other);
    Vector2D& operator*=(double scalar);
     
    double operator*=(Vector2D const& other);

//Déclaration des attributs, voir REPONSES P1.1 pour plus de détails
private:

    double x_;
    double y_;
};

//pas d'utilisation de namespace std dans les headers.
std::ostream& operator<<(std::ostream& out, Vector2D const& vec);

bool operator==(Vector2D const& vec1, Vector2D const& vec2);
bool operator!=(Vector2D const& vec1, Vector2D const& vec2);

Vector2D operator+(Vector2D const& vec1, Vector2D vec2);
Vector2D operator-(Vector2D vec1, Vector2D const& vec2);
double operator*(Vector2D vec1, Vector2D const& vec2);
Vector2D operator*(Vector2D vec, double scalar);
//commutativité de la loi de multiplication externe
Vector2D operator*(double scalar, Vector2D);
//retourne vecteur unitaire
Vector2D operator~(Vector2D const& vec);
//Remplace oppose()
Vector2D operator-(Vector2D const& vec);
