#pragma once
#include <ostream>
#include <array>

class AirCell {
    // Attributes
private:
    double z = 0.0;
    std::array<double, 3> velocity = {0.0, 0.0, 0.0};
    double enthalpy = 0.0;
    double temperature = 0.0;
    double pressure = 0.0;
    double vapourPressure = 0.0;     // partial pressure of water vapour
    double saturationPressure = 0.0; // saturation vapour pressure
    double humidity = 0.0;           // humidity ratio
    bool belowTerrain = false;

public:
    bool isCloudy() const;
    double velocitySquaredNorm() const;
    bool isBelowTerrain() const { return belowTerrain; }

    // Initialisation and setters
    void initialize(bool n);
    void update();
    void setZ(double z_);
    void setVelocity(std::array<double, 3> const& v);

    // Printing
    void printState() const;
    std::ostream& printState(std::ostream& out) const;

    // Accessors, used by the renderers and the tests
    double getEnthalpy() const { return enthalpy; }
    double getTemperature() const { return temperature; }
    double getPressure() const { return pressure; }
    double getVapourPressure() const { return vapourPressure; }
    double getSaturationPressure() const { return saturationPressure; }
};