#pragma once

#include <QOpenGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "SupportADessin.h"
#include "Systeme.h"


class VueOpenGL : public SupportADessin, protected QOpenGLFunctions {
public:
  ~VueOpenGL(); //Nécessaire pour libérer la mémoire des textures

  // méthodes de dessin héritées de SupportADessin
  virtual void dessine(Montagne const& montagne) override;
  virtual void dessine(Ciel const& ciel) override;

  // méthodes de (ré-)initialisation
  void init();
  void initializePosition();

  // méthode set
  void setProjection(QMatrix4x4 const& projection)
  { prog.setUniformValue("projection", projection); }

  // Méthodes set
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);
  
  // méthode utilitaire offerte pour simplifier
  void dessineSphere(QMatrix4x4 const& point_de_vue, double rouge = 1.0, double vert = 1.0, double bleu = 1.0);
  void dessineMont(QMatrix4x4 const& point_de_vue);

  double altitude(double i, double j) const;


private:
  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;

  //Texture
  GLuint nuages;
  GLuint montagne;
  GLuint gazon;
  GLuint neige;

  //Les dimensions du dessin
  std::array<int, 3> Cubes;

  // Caméra
  QMatrix4x4 matrice_vue;
};
