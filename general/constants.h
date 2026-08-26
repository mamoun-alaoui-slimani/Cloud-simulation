#pragma once

namespace Physics{
    constexpr double R(8.3144621); //constante des gaz parfaits
    constexpr double M_DRY_AIR(0.02896); //Masse molaire de l'air sec
    constexpr double G(9.81); //constante gravitationnelle sur Terre
    constexpr double M_WATER(0.0180153); //Masse molaire de la vapeur d'eau
    constexpr double T_INFINITY(284.5); //paramètre de la simulation représentant la température au sol au loin dans la plaine
    constexpr double H_INFINITY(((7/2.0) * (R * T_INFINITY)) / M_DRY_AIR); //l’enthalpy par unité de masse (pour un gaz parfait) à l’infini
    constexpr double TAU_INFINITY(0.008); //taux d'humidité initial
    constexpr double P_INFINITY(101325.0); //Pression de l'air au sol au loin dans la plaine
    constexpr double V_INFINITY(20.0); //velocity au bord
    constexpr double BERNOULLI_CONSTANT(((1/2.0) * V_INFINITY * V_INFINITY) + H_INFINITY);
    constexpr double P_REF(P_INFINITY); //Pas tout le temps égale à P_INFINITY, devra être modifiée en cas de changement
}