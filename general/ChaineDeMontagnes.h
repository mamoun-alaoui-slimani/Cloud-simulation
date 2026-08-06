
#pragma once
#include "Montagne.h"
#include <iostream>
#include <vector>
#include <memory>

/**
 * @brief Représentation d'une Chaine de Montagnes
 * 
 */
class ChaineDeMontagnes : public Montagne{
private:
std::vector<std::unique_ptr<Montagne>> chaine;

public:
ChaineDeMontagnes(std::vector<std::unique_ptr<Montagne>>& v);
~ChaineDeMontagnes() = default;

double altitude(double i, double j) const override;
std::vector<std::unique_ptr<Montagne>>& getChaine();
std::ostream& affiche(std::ostream& sortie) const override;
virtual void dessine_sur(SupportADessin& support) override;
};

std::ostream& operator<<(std::ostream& sortie, ChaineDeMontagnes const& chaine);
