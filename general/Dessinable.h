#pragma once

class SupportADessin;

/**
 * @brief Interface des objets capables de se dessiner sur un SupportADessin.
 *
 * Premiere moitie du double dispatch : l'objet ne sait pas se dessiner,
 * il sait seulement demander a un support de le dessiner.
 */
class Dessinable {
public:
  virtual void dessine_sur(SupportADessin& support) = 0;

  Dessinable()                             = default;
  virtual ~Dessinable()                    = default;
  Dessinable(Dessinable const&)            = default;
  Dessinable& operator=(Dessinable const&) = default;
  Dessinable(Dessinable&&)                 = default;
  Dessinable& operator=(Dessinable&&)      = default;
};
