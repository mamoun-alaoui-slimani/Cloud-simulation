#pragma once
#include <vector>
#include <array>
#include "AirCell.h"
#include "Grid3D.h"
#include "PotentialField.h"
#include "MountainChain.h"
#include "Drawable.h"
#include "Renderer.h"

class Sky : Grid3D<AirCell>, public Drawable {
public:
    Sky(PotentialField const& cp);
    Sky(double Lx, double Ly, double Lz, double lambda, double velocity);
    Sky();
    void drawOn(Renderer& renderer) override;
    std::array<int, 3> getCellCount() const { return cellCount; }
    bool isCloudy(int i, int j, int k) const;
    bool isBelowTerrain(int i, int j, int k) const;
    void update();

    std::ostream& printClouds(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, Sky const& sky);
