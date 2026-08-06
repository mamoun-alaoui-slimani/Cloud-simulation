#pragma once

#include "Dessinable.h"
#include "SupportADessin.h"

class Contenu : public Dessinable {
public:
  Contenu()
    : angle(0.0)
  {}
  virtual ~Contenu() = default;
  Contenu(Contenu const&)            = default;
  Contenu& operator=(Contenu const&) = default;
  Contenu(Contenu&&)                 = default; 
  Contenu& operator=(Contenu&&)      = default;

  //virtual void dessine_sur(SupportADessin& support) override
  //{ support.dessine(*this); }

  void evolue(double dt);

  // accesseur
  double infos() const { return angle; }

private:
  double angle; /* pour le mouvement ;
                   dans cet exemple, juste une rotation
                   au cours du temps                    */
};
