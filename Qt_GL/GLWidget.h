#pragma once

#include <QOpenGLWidget>     // OpenGL window
#include <QElapsedTimer>     // frame timing
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

  // Event handling
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;

  // Internal helpers
  void pause();

  // Timer
  int timerId;

  // used to evolve the objects with the right "dt"
  QElapsedTimer stopwatch;

  /* A reference, not a copy: System holds a MountainChain, hence a
   * vector<unique_ptr<Mountain>>, which is not copyable.
   * The System in main() outlives a.exec(). */
  System& system;
  OpenGLRenderer renderer;
};
