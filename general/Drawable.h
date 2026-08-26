#pragma once

class Renderer;

/**
 * @brief Interface des objets capables de se dessiner sur un Renderer.
 *
 * Premiere moitie du double dispatch : l'objet ne sait pas se dessiner,
 * il sait seulement demander a un renderer de le dessiner.
 */
class Drawable {
public:
  virtual void drawOn(Renderer& renderer) = 0;

  Drawable()                             = default;
  virtual ~Drawable()                    = default;
  Drawable(Drawable const&)            = default;
  Drawable& operator=(Drawable const&) = default;
  Drawable(Drawable&&)                 = default;
  Drawable& operator=(Drawable&&)      = default;
};
