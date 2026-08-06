/**
 * @file Montagne.h
 * @author Mamoun Alaoui Slimani
 * @brief Fichier d'en-tête pour la classe Montagne
 * @version 0.1
 * @date 2021-03-18
 */

#pragma once
#include <iostream>
#include "Dessinable.h"

/**
 * @brief Représentation d'une montagne
 */
class Montagne : Dessinable{
    protected:
    double i0, j0, hauteur, sigma_x, sigma_y;

    public:
    virtual double altitude(double i, double j) const = 0;
    Montagne(double i0, double j0, double h, double sx, double sy);
    Montagne() = default;
    virtual ~Montagne() = default;
    virtual std::ostream& affiche(std::ostream& sortie) const;
};

std::ostream& operator<<(std::ostream& sortie, Montagne const& m);