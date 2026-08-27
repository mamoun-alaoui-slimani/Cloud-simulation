#pragma once

#include <QOpenGLWidget> // OpenGL window
#include <QElapsedTimer> // frame timing
#include <atomic>
#include <thread>
#include "OpenGLRenderer.h"
#include "../general/System.h"

/**
 * @brief The OpenGL window: owns the view and drives the simulation.
 *
 * Solving the flow field takes tens of seconds. It runs on a worker
 * thread so the window stays responsive, and the scene is only drawn
 * once the solver has finished.
 */
class GLWidget : public QOpenGLWidget {
public:
    GLWidget(System& system, QWidget* parent = nullptr) : QOpenGLWidget(parent), system(system) {}
    ~GLWidget() override;

private:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    // Event handling
    void keyPressEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

    // Internal helpers
    void pause();

    // Timer
    int timerId = 0;

    // used to evolve the objects with the right "dt"
    QElapsedTimer stopwatch;

    /* The solver runs here. `solved` is the handoff: the worker only ever
     * sets it once, at the very end, and the UI thread reads nothing from
     * the system until it sees it set. */
    std::thread solver;
    std::atomic<bool> solved{false};

    /* A reference, not a copy: System holds a MountainChain, hence a
     * vector<unique_ptr<Mountain>>, which is not copyable.
     * The System in main() outlives a.exec(). */
    System& system;
    OpenGLRenderer renderer;
};
