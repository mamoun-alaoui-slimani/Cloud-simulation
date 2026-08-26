#pragma once

#include <QOpenGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QString>
#include "Renderer.h"
#include "System.h"

/**
 * @brief Rendu 3D temps réel du système, en OpenGL.
 *
 * Implémente Renderer : le modèle physique ne connaît pas cette
 * classe, il sait seulement demander à un renderer de le dessiner.
 */
class OpenGLRenderer : public Renderer, protected QOpenGLFunctions {
public:
  ~OpenGLRenderer() override; //Nécessaire pour libérer la mémoire des textures

  // méthodes de dessin héritées de Renderer
  void draw(Mountain const& mountain) override;
  void draw(Sky const& sky) override;

  // méthodes de (ré-)initialisation
  void init();
  void initializePosition();

  void setProjection(QMatrix4x4 const& projection)
  { program.bind(); program.setUniformValue("projection", projection); }

  // déplacements de la caméra
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);

private:
  // Altitudes de transition entre les trois textures du relief
  static constexpr double GRASS_ALTITUDE = 3.0;
  static constexpr double ROCK_ALTITUDE = 10.0;

  // Étendue et finesse de la nappe dessinée pour un amas nuageux
  static constexpr double CLOUD_RADIUS = 2.0;
  static constexpr double CLOUD_STEP   = 0.2;

  static QOpenGLTexture* loadTexture(QString const& path);

  /**
   * @brief Hauteur d'un point de la surface d'un amas nuageux.
   *
   * Forme purement décorative : elle donne au nuage un relief bosselé
   * plausible. Elle ne provient PAS de la simulation. Seule la position
   * de l'amas est physique : elle vient des cubes d'air que
   * Sky::isCloudy() a marqués comme condensés.
   */
  static double cloudSurface(double i, double j);

  /// Texture du relief selon l'altitude : gazon, roche, puis neige.
  QOpenGLTexture* terrainTexture(double altitude) const;

  /// Dessine un quadrilatère texturé de coin (i, j) et d'altitudes données.
  void drawTile(QOpenGLTexture* texture, double i, double j,
                    double z00, double z10, double z11, double z01);

  /// Dessine une nappe nuageuse centrée sur le point de renderer donné.
  void drawCloudPuff(QMatrix4x4 const& modelMatrix);

  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram program;

  // Textures (QOpenGLTexture remplace QGLContext::bindTexture, retiré en Qt 6)
  QOpenGLTexture* cloudTexture = nullptr;
  QOpenGLTexture* rockTexture  = nullptr;
  QOpenGLTexture* grassTexture  = nullptr;
  QOpenGLTexture* snowTexture  = nullptr;

  //Les dimensions du dessin
  std::array<int, 3> cellCount = {0, 0, 0};

  // Caméra
  QMatrix4x4 viewMatrix;
};
