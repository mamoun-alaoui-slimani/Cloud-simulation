#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include <iostream>
#include "GLWidget.h"


// ======================================================================
void GLWidget::initializeGL()
{
  renderer.init();
  timerId = startTimer(2500);
  system.start(renderer);
}

// ======================================================================
void GLWidget::resizeGL(int width, int height)
{

  glViewport(0, 0, width, height);

  QMatrix4x4 projection;
  projection.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
  renderer.setProjection(projection);
}

// ======================================================================
void GLWidget::paintGL()
{
  glClearColor(0.135, 0.206, 0.250, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  system.drawOn(renderer);
}


// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  constexpr double smallAngle(5.0); // en degrés
  constexpr double smallStep(1.0);

  switch (event->key()) {

  case Qt::Key_Left:
    renderer.rotate(smallAngle, 0.0, -1.0, 0.0);
    break;

  case Qt::Key_Right:
    renderer.rotate(smallAngle, 0.0, +1.0, 0.0);
    break;

  case Qt::Key_Up:
    renderer.rotate(smallAngle, -1.0, 0.0, 0.0);
    break;

  case Qt::Key_Down:
    renderer.rotate(smallAngle, +1.0, 0.0, 0.0);
    break;

  case Qt::Key_PageUp:
  case Qt::Key_W:
    renderer.translate(0.0, 0.0,  smallStep);
    break;

  case Qt::Key_PageDown:
  case Qt::Key_S:
    renderer.translate(0.0, 0.0, -smallStep);
    break;

  case Qt::Key_A:
    renderer.translate( smallStep, 0.0, 0.0);
    break;

  case Qt::Key_D:
    renderer.translate(-smallStep, 0.0, 0.0);
    break;

  case Qt::Key_R:
    renderer.translate(0.0, -smallStep, 0.0);
    break;

  case Qt::Key_F:
    renderer.translate(0.0,  smallStep, 0.0);
    break;

  case Qt::Key_Q:
    renderer.rotate(smallAngle, 0.0, 0.0, -1.0);
    break;

  case Qt::Key_E:
    renderer.rotate(smallAngle, 0.0, 0.0, +1.0);
    break;

  case Qt::Key_Home:
    renderer.initializePosition();
    break;

  case Qt::Key_Space:
	pause();
	break;
  };

  update(); // redessine
}

// ======================================================================
void GLWidget::timerEvent(QTimerEvent* event)
{
  Q_UNUSED(event);

  stopwatch.restart();
  system.update(renderer);

  update();
}

// ======================================================================
void GLWidget::pause()
{
  if (timerId == 0) {
	// dans ce cas le timer ne tourne pas alors on le lance
    timerId = startTimer(2500);
	stopwatch.restart();
  } else {
	// le timer tourne alors on l'arrête
	killTimer(timerId);
	timerId = 0;
  }
}
