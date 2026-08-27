#pragma once

#include <QOpenGLShaderProgram> // Qt wrapper around the GLSL program
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QString>
#include "Renderer.h"
#include "System.h"

/**
 * @brief Real-time 3D view of the system, in OpenGL.
 *
 * Implements Renderer: the physics model knows nothing about this
 * class, it only knows how to ask a renderer to draw it.
 */
class OpenGLRenderer : public Renderer, protected QOpenGLFunctions {
public:
  ~OpenGLRenderer() override; //frees the textures

  // drawing methods inherited from Renderer
  void draw(Mountain const& mountain) override;
  void draw(Sky const& sky) override;

  // (re-)initialisation
  void init();
  void initializePosition();

  void setProjection(QMatrix4x4 const& projection)
  { program.bind(); program.setUniformValue("projection", projection); }

  // camera movement
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);

private:
  // Altitudes at which the terrain texture changes
  static constexpr double GRASS_ALTITUDE = 3.0;
  static constexpr double ROCK_ALTITUDE = 10.0;

  // Extent and resolution of the sheet drawn for one cloud puff
  static constexpr double CLOUD_RADIUS = 2.0;
  static constexpr double CLOUD_STEP   = 0.2;

  static QOpenGLTexture* loadTexture(QString const& path);

  /**
   * @brief Height of a point on the surface of a cloud puff.
   *
   * Purely decorative: it gives a puff a plausible lumpy surface. It
   * does NOT come from the simulation. What is physical is only WHERE
   * a puff appears, which comes from the air cells that
   * Sky::isCloudy() marks as condensed.
   */
  static double cloudSurface(double i, double j);

  /// Terrain texture for a given altitude: grass, then rock, then snow.
  QOpenGLTexture* terrainTexture(double altitude) const;

  /// Draws one textured quad with corner (i, j) and the four given altitudes.
  void drawTile(QOpenGLTexture* texture, double i, double j,
                    double z00, double z10, double z11, double z01);

  /// Draws one cloud sheet, positioned by the given model matrix.
  void drawCloudPuff(QMatrix4x4 const& modelMatrix);

  // The GLSL program, wrapped by Qt
  QOpenGLShaderProgram program;

  // Textures (QOpenGLTexture replaces QGLContext::bindTexture, removed in Qt 6)
  QOpenGLTexture* cloudTexture = nullptr;
  QOpenGLTexture* rockTexture  = nullptr;
  QOpenGLTexture* grassTexture  = nullptr;
  QOpenGLTexture* snowTexture  = nullptr;

  // Grid dimensions being drawn
  std::array<int, 3> cellCount = {0, 0, 0};

  // Camera
  QMatrix4x4 viewMatrix;
};
