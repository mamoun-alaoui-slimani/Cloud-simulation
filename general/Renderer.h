#pragma once

class Mountain;
class Sky;

/**
 * @brief Interface d'un renderer de dessin (console, renderer OpenGL, fichier...).
 *
 * Seconde moitie du double dispatch : une surcharge de draw() par type
 * dessinable. Ajouter un rendu ne demande aucune modification du modele.
 */
class Renderer
{
public:
  virtual void draw(Mountain const& mountain) = 0;
  virtual void draw(Sky const& sky) = 0;

  Renderer()                                 = default;
  virtual ~Renderer()                        = default;
  // on ne copie pas les supports, mais on peut les deplacer
  Renderer(Renderer const&)            = delete;
  Renderer& operator=(Renderer const&) = delete;
  Renderer(Renderer&&)                 = default;
  Renderer& operator=(Renderer&&)      = default;
};
