#pragma once

#include <iostream>
#include "SupportADessin.h"

/**
 * @brief Rendu console du système.
 *
 * Le flot de sortie est injecté, ce qui permet d'écrire ailleurs que sur
 * std::cout (un fichier, ou un std::ostringstream dans les tests).
 */
class TextViewer : public SupportADessin {
public:
  explicit TextViewer(std::ostream& flot = std::cout)
    : flot(flot)
  {}

  void dessine(Montagne const& montagne) override;
  void dessine(Ciel const& ciel) override;

private:
  std::ostream& flot;
};
