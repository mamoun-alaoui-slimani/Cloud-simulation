#pragma once

#include <QOpenGLWidget>        // Classe pour faire une fenêtre OpenGL
#include <QElapsedTimer>    // Classe pour gérer le temps
#include "OpenGLRenderer.h"
#include "../general/System.h"

class GLWidget : public QOpenGLWidget
{
public:
  GLWidget(System& system, QWidget* parent = nullptr)
    : QOpenGLWidget(parent), system(system)
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
  QElapsedTimer stopwatch;

  /* Référence, pas une copie : System contient une MountainChain,
   * donc un vector<unique_ptr<Mountain>>, qui n'est pas copiable.
   * Le System de main() vit jusqu'à la fin de a.exec(). */
  System& system;
  OpenGLRenderer renderer;
};
