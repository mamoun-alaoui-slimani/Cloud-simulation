/**
 * @file Mountain.h
 * @author Mamoun Alaoui Slimani
 * @brief Fichier d'en-tête pour la classe Mountain
 * @version 0.1
 * @date 2021-03-18
 */

#pragma once
#include <iostream>
#include "Drawable.h"

/**
 * @brief Représentation d'une mountain
 */
class Mountain : Drawable{
    protected:
    double i0, j0, height, sigmaX, sigmaY;

    public:
    virtual double altitude(double i, double j) const = 0;
    Mountain(double i0, double j0, double h, double sx, double sy);
    Mountain() = default;
    virtual ~Mountain() = default;
    virtual std::ostream& print(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, Mountain const& m);