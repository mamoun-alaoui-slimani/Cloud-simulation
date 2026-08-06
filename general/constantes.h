#pragma once

namespace Physique{
    constexpr double R(8.3144621); //constante des gaz parfaits
    constexpr double M_air_sec(0.02896); //Masse molaire de l'air sec
    constexpr double g(9.81); //constante gravitationnelle sur Terre
    constexpr double M_eau(0.0180153); //Masse molaire de la vapeur d'eau
    constexpr double T_infini(284.5); //paramètre de la simulation représentant la température au sol au loin dans la plaine
    constexpr double h_infini(((7/2.0) * (R * T_infini)) / M_air_sec); //l’enthalpie par unité de masse (pour un gaz parfait) à l’infini
    constexpr double tau_infini(0.008); //taux d'humidité initial
    constexpr double P_infini(101325.0); //Pression de l'air au sol au loin dans la plaine
    constexpr double v_infini(20.0); //vitesse au bord
    constexpr double cte(((1/2.0) * v_infini * v_infini) + h_infini);
    constexpr double P_ref(P_infini); //Pas tout le temps égale à P_infini, devra être modifiée en cas de changement
}