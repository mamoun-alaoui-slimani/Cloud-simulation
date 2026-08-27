#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include <iostream>
#include "GLWidget.h"

// ======================================================================
GLWidget::~GLWidget() {
    if (solver.joinable())
        solver.join();
}

// ======================================================================
void GLWidget::initializeGL() {
    renderer.init();

    /* The flow field takes tens of seconds to relax. Solving it here on the
       UI thread would freeze the window; instead the work goes to a worker
       and the timer below polls for its completion. */
    setWindowTitle("Cloud simulation - solving the flow field...");
    solver = std::thread([this] {
        system.solve();
        solved = true;
    });

    timerId = startTimer(2500);
}

// ======================================================================
void GLWidget::resizeGL(int width, int height) {

    glViewport(0, 0, width, height);

    QMatrix4x4 projection;
    projection.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
    renderer.setProjection(projection);
}

// ======================================================================
void GLWidget::paintGL() {
    glClearColor(0.135, 0.206, 0.250, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Nothing to draw until the solver has produced a field.
    if (not solved)
        return;

    system.drawOn(renderer);
}

// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event) {
    constexpr double smallAngle(5.0); // in degrees
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
        renderer.translate(0.0, 0.0, smallStep);
        break;

    case Qt::Key_PageDown:
    case Qt::Key_S:
        renderer.translate(0.0, 0.0, -smallStep);
        break;

    case Qt::Key_A:
        renderer.translate(smallStep, 0.0, 0.0);
        break;

    case Qt::Key_D:
        renderer.translate(-smallStep, 0.0, 0.0);
        break;

    case Qt::Key_R:
        renderer.translate(0.0, -smallStep, 0.0);
        break;

    case Qt::Key_F:
        renderer.translate(0.0, smallStep, 0.0);
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
void GLWidget::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event);

    if (not solved)
        return; // the solver is still running

    if (solver.joinable()) {
        solver.join();
        setWindowTitle("Cloud simulation");
    }

    stopwatch.restart();
    system.step(); // physics only; drawing happens in paintGL

    update();
}

// ======================================================================
void GLWidget::pause() {
    if (timerId == 0) {
        // the timer is not running, so start it
        timerId = startTimer(2500);
        stopwatch.restart();
    } else {
        // the timer is running, so stop it
        killTimer(timerId);
        timerId = 0;
    }
}
