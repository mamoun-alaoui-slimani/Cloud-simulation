#pragma once

#include <QOpenGLWidget>        // Classe pour faire une fenêtre OpenGL
#include <QElapsedTimer>    // Classe pour gérer le temps
#include "vue_opengl.h"
#include "../general/Systeme.h"

class GLWidget : public QOpenGLWidget
{
public:
  GLWidget(Systeme& systeme, QWidget* parent = nullptr)
    : QOpenGLWidget(parent), systeme(systeme)
  {}
  virtual ~GLWidget() = default;

private:
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;

  // Méthodes de gestion interne
  void pause();

  // Timer
  int timerId;

  // pour faire évoluer les objets avec le bon "dt"
  QElapsedTimer chronometre;

  /* Référence, pas une copie : Systeme contient une ChaineDeMontagnes,
   * donc un vector<unique_ptr<Montagne>>, qui n'est pas copiable.
   * Le Systeme de main() vit jusqu'à la fin de a.exec(). */
  Systeme& systeme;
  VueOpenGL vue;
};
