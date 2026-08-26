
#include <iostream>
#include "Vector2D.h" //besoin de la définition de la classe.
#include <cmath> //pour la fonction abs()
using namespace std;

/**
 * @brief Construit un nouvel objet Vector2D
 * 
 * @param x abscisse du vecteur
 * @param y ordonnée du vecteur
 */
Vector2D::Vector2D(double x, double y): x_(x), y_(y) {}

/**
 * @brief Permet de modifier la valeur d'une coordonnée du Vecteur
 * 
 * @param x nouvelle abscisse du vecteur
 * @param y nouvelle ordonnée du vecteur
 */
void Vector2D::setCoords(double x, double y){ 
   x_ = x;
   y_ = y;
}

/**
 * Permet l'affichage sur "cout" d'un objet Vector2D
 * 
 * @param out est de type ostream& pour la surcharge de operator<<
 * @return ostream& pour pouvoir afficher plusieurs Vecteurs avec cout
 */
ostream& Vector2D::print(ostream& out) const { //const car pas de modif de x et y
    out << x_ <<" "<< y_;
    return out;
}

/**
 * @brief Teste l'égalité de l'objet Vector2D courant à un autre objet similaire
 * 
 * @param other Vecteur à comparer
 * @param precision precision souhaitée pour la comparaison
 * @return true si les deux vecteurs sont égaux. false sinon
 */
bool Vector2D::compare(Vector2D const& other, double precision) const{
    return ((abs(x_ - other.x_) <= precision) and (abs(y_ - other.y_) <= precision));
}

/**
 * @brief Surcharge de += pour l'addition de deux vecteurs (Vector2D). Surcharge interne.
 * @param other
 * @return Vector2D | addition de l'instance courante avec other. donc pas de nouvel objet créé donc référence.
 */
Vector2D& Vector2D::operator+=(Vector2D const& other){
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

/**
 * @brief Surcharge de -= pour la soustraction. Surcharge interne.
 * @param other Vector2D
 * @return Vector2D | Soustraction de l'instance courante avec other
 */
Vector2D& Vector2D::operator-=(Vector2D const& other){
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
}

/**
 * @brief Surcharge de *= pour la loi de multiplication externe. Surcharge interne.
 * @param scalar
 * @return Vector2D | produit de l'instance courante (Vector2D) avec scalar
 */
Vector2D& Vector2D::operator*=(double scalar){
    x_ *= scalar;
    y_ *= scalar;
    return *this;
}

/**
 * @brief Surcharge de *= pour le produit scalar. Surcharge interne.
 * @param other
 * @return double | produit scalar de l'instance courante avec other
 */
double Vector2D::operator*=(Vector2D const& other){
    return x_ * other.x_ + y_ * other.y_;
}

/**
 * @brief Calcul le carré de la norm du vecteur courant
 * 
 * @return double le carré de la norm du vecteur courant
 */
double Vector2D::squaredNorm() const {
    return x_ * x_ + y_ * y_;
}

/**
 * @brief Calcul la norm du vecteur courant
 * 
 * @return double la norm du vecteur courant
 */
double Vector2D::norm() const {
    return sqrt(squaredNorm());
}

/**
 * @brief Surcharge de l'opérateur d'affichage.
 * 
 * @param out 
 * @param vec 
 * @return ostream& 
 * La méthode publique print() a été gardée pour permettre un accès facile aux attributs de Vector2D ceux-ci étant privé. Permet une surcharge externe de <<.
 */
ostream& operator<<(ostream& out, Vector2D const& vec){
    return vec.print(out);
}
/**
 * @brief Surcharge de l'opérateur ==
 * 
 * @param vec2
 * @param vec1 
 * @return la méthode publique compare() a été gardée pour avoir une surcharge externe de l'opérateur == tout en ayant un accès facile aux attributs privés de Vector2D. La garder permet aussi de changer plus facilement le paramètre de précision.
 */
bool operator==(Vector2D const& vec1, Vector2D const& vec2){ 
    return vec1.compare(vec2); 
}

/**
 * @brief Surcharge de l'opérateur != pour des manipulations plus simples des vecteurs dans des évaluations de type bool
 * @param vec1
 * @param vec2
 * @return doit retourner faux si les vecteurs sont les mêmes, vrai sinon.
 */
bool operator!=(Vector2D const& vec1, Vector2D const& vec2){
    if(vec1 == vec2) return false;
    else return true;
}

/**
 * @brief Surcharge de + 
 * @param vec1 
 * @param vec2 celui à qui on ajoute vec1 donc pas de const&
 * @return Vecteur 2D (nouvel objet) appelle operator += pour optimisation et cohérence
 */
Vector2D operator+(Vector2D const& vec1, Vector2D vec2){
    return vec2.operator+=(vec1);
}
/**
 * @brief Surcharge de -
 * @param vec1 celui à qui on soustrait vec2, donc pas de const&
 * @param vec2
 * @return appelle operator -= pour optimisation et cohérence
 */
Vector2D operator-(Vector2D vec1, Vector2D const& vec2){
    return vec1.operator-=(vec2);
}
/**
 * @brief Surcharge de * pour le produit scalar 
 * @param vec1
 * @param vec2
 * @return appelle operator *= pour optimisation et cohérence
 */
double operator*(Vector2D vec1, Vector2D const& vec2){
    return vec1.operator*=(vec2);
}
/**
 * @brief Surcharge de * pour multiplication d'un scalar avec un vecteur
 * @param vec
 * @param scalar
 * @return appelle operator *= pour la multiplication avec avec scalar pour optimisation et cohérence
 */
Vector2D operator*(Vector2D vec, double scalar){
    return vec.operator*=(scalar);
}
/**
 * @brief Surcharge de * pour multiplication d'un scalar avec un vecteur trroisième surcharge nécessaire pour scalar * vec plutôt  que seulement vec * scalar.
 * @param scalar
 * @param vec
 * @return appelle operator *= pour la multiplication avec avec scalar pour optimisation et cohérence
 */
Vector2D operator*(double scalar, Vector2D vec){
    return vec.operator*=(scalar);
}

/**
 * @brief Surcharge de l'opérateur unaire ~
 * @param vec
 * @return Vector2D retourne un vecteur de norm 1 qui a la même direction et sens que le vecteur pris en paramètre.
 */
Vector2D operator~(Vector2D const& vec){
    double norm(vec.norm());
    //Evite la division par 0 dans le cas où le vecteur est le vecteur nul
    if(norm != 0) return vec * (1 / norm); 
    else {
        return vec; //si le vecteur est nul on retourne l'instance courante.
    }
}

/**
 * @brief surcharge de " - " un seul paramètre
 * @param vec
 * @return Vector2D retourne l'opposé du vecteur passé en paramètre.
 */ 
Vector2D operator-(Vector2D const& vec){
    Vector2D opposite;
    opposite -= vec;
    return opposite;
}