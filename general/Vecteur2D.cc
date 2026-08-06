
#include <iostream>
#include "Vecteur2D.h" //besoin de la définition de la classe.
#include <cmath> //pour la fonction abs()
using namespace std;

/**
 * @brief Construit un nouvel objet Vecteur2D
 * 
 * @param x abscisse du vecteur
 * @param y ordonnée du vecteur
 */
Vecteur2D::Vecteur2D(double x, double y): x_(x), y_(y) {}

/**
 * @brief Permet de modifier la valeur d'une coordonnée du Vecteur
 * 
 * @param x nouvelle abscisse du vecteur
 * @param y nouvelle ordonnée du vecteur
 */
void Vecteur2D::set_coord(double x, double y){ 
   x_ = x;
   y_ = y;
}

/**
 * Permet l'affichage sur "cout" d'un objet Vecteur2D
 * 
 * @param sortie est de type ostream& pour la surcharge de operator<<
 * @return ostream& pour pouvoir afficher plusieurs Vecteurs avec cout
 */
ostream& Vecteur2D::affiche(ostream& sortie) const { //const car pas de modif de x et y
    sortie << x_ <<" "<< y_;
    return sortie;
}

/**
 * @brief Teste l'égalité de l'objet Vecteur2D courant à un autre objet similaire
 * 
 * @param ACompare Vecteur à comparer
 * @param precis precision souhaitée pour la comparaison
 * @return true si les deux vecteurs sont égaux. false sinon
 */
bool Vecteur2D::compare(Vecteur2D const& ACompare, double precis) const{
    return ((abs(x_ - ACompare.x_) <= precis) and (abs(y_ - ACompare.y_) <= precis));
}

/**
 * @brief Surcharge de += pour l'addition de deux vecteurs (Vecteur2D). Surcharge interne.
 * @param Add
 * @return Vecteur2D | addition de l'instance courante avec Add. donc pas de nouvel objet créé donc référence.
 */
Vecteur2D& Vecteur2D::operator+=(Vecteur2D const& Add){
    x_ += Add.x_;
    y_ += Add.y_;
    return *this;
}

/**
 * @brief Surcharge de -= pour la soustraction. Surcharge interne.
 * @param Sous Vecteur2D
 * @return Vecteur2D | Soustraction de l'instance courante avec Sous
 */
Vecteur2D& Vecteur2D::operator-=(Vecteur2D const& Sous){
    x_ -= Sous.x_;
    y_ -= Sous.y_;
    return *this;
}

/**
 * @brief Surcharge de *= pour la loi de multiplication externe. Surcharge interne.
 * @param scalaire
 * @return Vecteur2D | produit de l'instance courante (Vecteur2D) avec scalaire
 */
Vecteur2D& Vecteur2D::operator*=(double scalaire){
    x_ *= scalaire;
    y_ *= scalaire;
    return *this;
}

/**
 * @brief Surcharge de *= pour le produit scalaire. Surcharge interne.
 * @param VecScal
 * @return double | produit scalaire de l'instance courante avec VecScal
 */
double Vecteur2D::operator*=(Vecteur2D const& VecScal){
    return x_ * VecScal.x_ + y_ * VecScal.y_;
}

/**
 * @brief Calcul le carré de la norme du vecteur courant
 * 
 * @return double le carré de la norme du vecteur courant
 */
double Vecteur2D::norme2() const {
    return x_ * x_ + y_ * y_;
}

/**
 * @brief Calcul la norme du vecteur courant
 * 
 * @return double la norme du vecteur courant
 */
double Vecteur2D::norme() const {
    return sqrt(norme2());
}

/**
 * @brief Surcharge de l'opérateur d'affichage.
 * 
 * @param sortie 
 * @param vec 
 * @return ostream& 
 * La méthode publique affiche() a été gardée pour permettre un accès facile aux attributs de Vecteur2D ceux-ci étant privé. Permet une surcharge externe de <<.
 */
ostream& operator<<(ostream& sortie, Vecteur2D const& vec){
    return vec.affiche(sortie);
}
/**
 * @brief Surcharge de l'opérateur ==
 * 
 * @param vec2
 * @param vec1 
 * @return la méthode publique compare() a été gardée pour avoir une surcharge externe de l'opérateur == tout en ayant un accès facile aux attributs privés de Vecteur2D. La garder permet aussi de changer plus facilement le paramètre de précision.
 */
bool operator==(Vecteur2D const& vec1, Vecteur2D const& vec2){ 
    return vec1.compare(vec2); 
}

/**
 * @brief Surcharge de l'opérateur != pour des manipulations plus simples des vecteurs dans des évaluations de type bool
 * @param vec1
 * @param vec2
 * @return doit retourner faux si les vecteurs sont les mêmes, vrai sinon.
 */
bool operator!=(Vecteur2D const& vec1, Vecteur2D const& vec2){
    if(vec1 == vec2) return false;
    else return true;
}

/**
 * @brief Surcharge de + 
 * @param vec1 
 * @param vec2 celui à qui on ajoute vec1 donc pas de const&
 * @return Vecteur 2D (nouvel objet) appelle operator += pour optimisation et cohérence
 */
Vecteur2D operator+(Vecteur2D const& vec1, Vecteur2D vec2){
    return vec2.operator+=(vec1);
}
/**
 * @brief Surcharge de -
 * @param vec1 celui à qui on soustrait vec2, donc pas de const&
 * @param vec2
 * @return appelle operator -= pour optimisation et cohérence
 */
Vecteur2D operator-(Vecteur2D vec1, Vecteur2D const& vec2){
    return vec1.operator-=(vec2);
}
/**
 * @brief Surcharge de * pour le produit scalaire 
 * @param vec1
 * @param vec2
 * @return appelle operator *= pour optimisation et cohérence
 */
double operator*(Vecteur2D vec1, Vecteur2D const& vec2){
    return vec1.operator*=(vec2);
}
/**
 * @brief Surcharge de * pour multiplication d'un scalaire avec un vecteur
 * @param vec
 * @param scalaire
 * @return appelle operator *= pour la multiplication avec avec scalaire pour optimisation et cohérence
 */
Vecteur2D operator*(Vecteur2D vec, double scalaire){
    return vec.operator*=(scalaire);
}
/**
 * @brief Surcharge de * pour multiplication d'un scalaire avec un vecteur trroisième surcharge nécessaire pour scalaire * vec plutôt  que seulement vec * scalaire.
 * @param scalaire
 * @param vec
 * @return appelle operator *= pour la multiplication avec avec scalaire pour optimisation et cohérence
 */
Vecteur2D operator*(double scalaire, Vecteur2D vec){
    return vec.operator*=(scalaire);
}

/**
 * @brief Surcharge de l'opérateur unaire ~
 * @param vec
 * @return Vecteur2D retourne un vecteur de norme 1 qui a la même direction et sens que le vecteur pris en paramètre.
 */
Vecteur2D operator~(Vecteur2D const& vec){
    double norme(vec.norme());
    //Evite la division par 0 dans le cas où le vecteur est le vecteur nul
    if(norme != 0) return vec * (1 / norme); 
    else {
        return vec; //si le vecteur est nul on retourne l'instance courante.
    }
}

/**
 * @brief surcharge de " - " un seul paramètre
 * @param vec
 * @return Vecteur2D retourne l'opposé du vecteur passé en paramètre.
 */ 
Vecteur2D operator-(Vecteur2D const& vec){
    Vecteur2D opp;
    opp -= vec;
    return opp;
}