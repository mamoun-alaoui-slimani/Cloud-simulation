
#include <iostream>
#include "Vector2D.h"
#include <cmath> //for std::abs()

/**
 * @brief Builds a vector from its two coordinates.
 *
 * @param x abscissa
 * @param y ordinate
 */
Vector2D::Vector2D(double x, double y) : x_(x), y_(y) {}

/**
 * @brief Sets both coordinates.
 *
 * @param x new abscissa
 * @param y new ordinate
 */
void Vector2D::setCoords(double x, double y) {
    x_ = x;
    y_ = y;
}

/**
 * Prints the vector on a stream.
 *
 * @param out output stream
 * @return the stream, so several vectors can be chained
 */
std::ostream& Vector2D::print(std::ostream& out) const { // const: x and y are not modified
    out << x_ << " " << y_;
    return out;
}

/**
 * @brief Compares this vector to another, within a tolerance.
 *
 * @param other vector to compare against
 * @param precision tolerance of the comparison
 * @return true when the two vectors are equal within that tolerance
 */
bool Vector2D::compare(Vector2D const& other, double precision) const {
    return ((std::abs(x_ - other.x_) <= precision) and (std::abs(y_ - other.y_) <= precision));
}

/**
 * @brief Adds another vector to this one.
 * @param other
 * @return this vector, modified
 */
Vector2D& Vector2D::operator+=(Vector2D const& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

/**
 * @brief Subtracts another vector from this one.
 * @param other Vector2D
 * @return this vector, modified
 */
Vector2D& Vector2D::operator-=(Vector2D const& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
}

/**
 * @brief Scales this vector by a scalar.
 * @param scalar
 * @return this vector, modified
 */
Vector2D& Vector2D::operator*=(double scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
}

/**
 * @brief Dot product with another vector.
 *
 * A named method rather than an operator*= overload: compound assignment
 * is expected to modify the object and return a reference to it, not to
 * return an unrelated scalar.
 *
 * @param other the other vector
 * @return the dot product
 */
double Vector2D::dot(Vector2D const& other) const {
    return x_ * other.x_ + y_ * other.y_;
}

/**
 * @brief Squared norm of this vector.
 *
 * @return the squared norm
 */
double Vector2D::squaredNorm() const {
    return x_ * x_ + y_ * y_;
}

/**
 * @brief Norm of this vector.
 *
 * @return the norm
 */
double Vector2D::norm() const {
    return std::sqrt(squaredNorm());
}

/**
 * @brief Stream insertion operator.
 *
 * @param out
 * @param vec
 * @return std::ostream&
 * print() stays public so the private attributes remain reachable.
 */
std::ostream& operator<<(std::ostream& out, Vector2D const& vec) {
    return vec.print(out);
}
/**
 * @brief Equality, delegating to compare().
 *
 * @param vec2
 * @param vec1
 * @return true when the vectors are equal
 */
bool operator==(Vector2D const& vec1, Vector2D const& vec2) {
    return vec1.compare(vec2);
}

/**
 * @brief Inequality.
 * @param vec1
 * @param vec2
 * @return false when the vectors are the same, true otherwise
 */
bool operator!=(Vector2D const& vec1, Vector2D const& vec2) {
    if (vec1 == vec2)
        return false;
    else
        return true;
}

/**
 * @brief Vector addition.
 * @param vec1
 * @param vec2 taken by value, since it receives the sum
 * @return the sum, computed through operator+=
 */
Vector2D operator+(Vector2D const& vec1, Vector2D vec2) {
    return vec2.operator+=(vec1);
}
/**
 * @brief Surcharge de -
 * @param vec1 taken by value, since it receives the difference
 * @param vec2
 * @return the difference, computed through operator-=
 */
Vector2D operator-(Vector2D vec1, Vector2D const& vec2) {
    return vec1.operator-=(vec2);
}
/**
 * @brief Dot product of two vectors.
 * @param vec1
 * @param vec2
 * @return the dot product
 */
double operator*(Vector2D vec1, Vector2D const& vec2) {
    return vec1.dot(vec2);
}
/**
 * @brief Vector times scalar.
 * @param vec
 * @param scalar
 * @return the scaled vector, computed through operator*=
 */
Vector2D operator*(Vector2D vec, double scalar) {
    return vec.operator*=(scalar);
}
/**
 * @brief Scalar times vector, so multiplication commutes.
 * @param scalar
 * @param vec
 * @return the scaled vector, computed through operator*=
 */
Vector2D operator*(double scalar, Vector2D vec) {
    return vec.operator*=(scalar);
}

/**
 * @brief Unary ~ : the unit vector with the same direction.
 * @param vec
 * @return a vector of norm 1, or the zero vector unchanged
 */
Vector2D operator~(Vector2D const& vec) {
    double norm(vec.norm());
    // guards against dividing by zero for the null vector
    if (norm != 0)
        return vec * (1 / norm);
    else {
        return vec; // the null vector is returned unchanged
    }
}

/**
 * @brief Unary minus.
 * @param vec
 * @return the opposite vector
 */
Vector2D operator-(Vector2D const& vec) {
    Vector2D opposite;
    opposite -= vec;
    return opposite;
}