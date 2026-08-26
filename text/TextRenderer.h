#pragma once

#include <iostream>
#include "Renderer.h"

/**
 * @brief Rendu console du système.
 *
 * Le stream de out est injecté, ce qui permet d'écrire ailleurs que sur
 * std::cout (un fichier, ou un std::ostringstream dans les tests).
 */
class TextRenderer : public Renderer {
public:
  explicit TextRenderer(std::ostream& stream = std::cout)
    : stream(stream)
  {}

  void draw(Mountain const& mountain) override;
  void draw(Sky const& sky) override;

private:
  std::ostream& stream;
};
