#pragma once

class Montagne;
class Ciel;

/**
 * @brief Interface d'un support de dessin (console, ecran OpenGL, fichier...).
 *
 * Seconde moitie du double dispatch : une surcharge de dessine() par type
 * dessinable. Ajouter un rendu ne demande aucune modification du modele.
 */
class SupportADessin
{
public:
  virtual void dessine(Montagne const& montagne) = 0;
  virtual void dessine(Ciel const& ciel) = 0;

  SupportADessin()                                 = default;
  virtual ~SupportADessin()                        = default;
  // on ne copie pas les supports, mais on peut les deplacer
  SupportADessin(SupportADessin const&)            = delete;
  SupportADessin& operator=(SupportADessin const&) = delete;
  SupportADessin(SupportADessin&&)                 = default;
  SupportADessin& operator=(SupportADessin&&)      = default;
};
