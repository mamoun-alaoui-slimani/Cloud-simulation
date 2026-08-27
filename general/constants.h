#pragma once

/**
 * @brief Physical constants and simulation parameters.
 *
 * The "infinity" suffix denotes conditions in the free stream, far from
 * the mountain, which set the boundary conditions of the simulation.
 */
namespace Physics{
    constexpr double R(8.3144621);        //ideal gas constant
    constexpr double M_DRY_AIR(0.02896);  //molar mass of dry air
    constexpr double G(9.81);             //gravitational acceleration on Earth
    constexpr double M_WATER(0.0180153);  //molar mass of water vapour

    constexpr double T_INFINITY(284.5);   //ground temperature far out in the plain
    constexpr double P_INFINITY(101325.0);//air pressure far out in the plain
    constexpr double V_INFINITY(20.0);    //free-stream wind speed
    constexpr double TAU_INFINITY(0.008); //initial humidity ratio

    //enthalpy per unit mass of an ideal gas, at infinity
    constexpr double H_INFINITY(((7/2.0) * (R * T_INFINITY)) / M_DRY_AIR);

    //conserved along a streamline: kinetic energy plus enthalpy
    constexpr double BERNOULLI_CONSTANT(((1/2.0) * V_INFINITY * V_INFINITY) + H_INFINITY);

    //not always equal to P_INFINITY; update it if the reference changes
    constexpr double P_REF(P_INFINITY);
}
