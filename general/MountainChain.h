
#pragma once
#include "Mountain.h"
#include <iostream>
#include <vector>
#include <memory>

/**
 * @brief Représentation d'une Chaine de Montagnes
 * 
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
