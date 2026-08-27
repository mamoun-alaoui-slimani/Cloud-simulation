
#pragma once
#include "Mountain.h"
#include <iostream>
#include <vector>
#include <memory>

/**
 * @brief A chain of mountains, which is itself a Mountain (Composite).
 *
 * Because a chain IS a Mountain and CONTAINS mountains, chains nest
 * arbitrarily and a nested chain behaves exactly like a single peak.
 */
class MountainChain : public Mountain{
private:
std::vector<std::unique_ptr<Mountain>> chain;

public:
MountainChain(std::vector<std::unique_ptr<Mountain>>& v);
~MountainChain() = default;

double altitude(double i, double j) const override;
std::vector<std::unique_ptr<Mountain>>& getChain();
std::ostream& print(std::ostream& out) const override;
virtual void drawOn(Renderer& renderer) override;
};

std::ostream& operator<<(std::ostream& out, MountainChain const& chain);
