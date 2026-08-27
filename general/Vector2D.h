
#pragma once 
#include <iostream>

/**
 * @brief A mathematical vector in two dimensions.
 * 
 */
class Vector2D{

public:

//Instance methods
    Vector2D(double x = 0.0, double y = 0.0); //defaults give a default constructor
    void setCoords(double NVx, double NVy);
    std::ostream& print(std::ostream& out) const;
//Getters
    double getX() const {return x_;}
    double getY() const {return y_;}
//Vector operations
    bool compare(Vector2D const& other, double precision = 1e-10) const; 
    double squaredNorm() const;
    double norm() const;

    Vector2D& operator+=(Vector2D const& other);
    Vector2D& operator-=(Vector2D const& other);
    Vector2D& operator*=(double scalar);
     
    double operator*=(Vector2D const& other);

//Attributes
private:

    double x_;
    double y_;
};

//no using-directive for std in a header
std::ostream& operator<<(std::ostream& out, Vector2D const& vec);

bool operator==(Vector2D const& vec1, Vector2D const& vec2);
bool operator!=(Vector2D const& vec1, Vector2D const& vec2);

Vector2D operator+(Vector2D const& vec1, Vector2D vec2);
Vector2D operator-(Vector2D vec1, Vector2D const& vec2);
double operator*(Vector2D vec1, Vector2D const& vec2);
Vector2D operator*(Vector2D vec, double scalar);
//commutativity of scalar multiplication
Vector2D operator*(double scalar, Vector2D);
//returns the unit vector
Vector2D operator~(Vector2D const& vec);
//replaces oppose()
Vector2D operator-(Vector2D const& vec);
