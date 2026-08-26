#pragma once

#include <QOpenGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QString>
#include "SupportADessin.h"
#include "Systeme.h"

/**
 * @brief Rendu 3D temps réel du système, en OpenGL.
 *
 * Implémente SupportADessin : le modèle physique ne connaît pas cette
 * classe, il sait seulement demander à un support de le dessiner.
 */
class VueOpenGL : public SupportADessin, protected QOpenGLFunctions {
public:
  ~VueOpenGL() override; //Nécessaire pour libérer la mémoire des textures

  // méthodes de dessin héritées de SupportADessin
  void dessine(Montagne const& montagne) override;
  void dessine(Ciel const& ciel) override;

  // méthodes de (ré-)initialisation
  void init();
  void initializePosition();

  void setProjection(QMatrix4x4 const& projection)
  { prog.bind(); prog.setUniformValue("projection", projection); }

  // déplacements de la caméra
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);

private:
  // Altitudes de transition entre les trois textures du relief
  static constexpr double ALTITUDE_GAZON = 3.0;
  static constexpr double ALTITUDE_ROCHE = 10.0;

  // Étendue et finesse de la nappe dessinée pour un amas nuageux
  static constexpr double RAYON_NUAGE = 2.0;
  static constexpr double PAS_NUAGE   = 0.2;

  static QOpenGLTexture* chargeTexture(QString const& chemin);

  /**
   * @brief Hauteur d'un point de la surface d'un amas nuageux.
   *
   * Forme purement décorative : elle donne au nuage un relief bosselé
   * plausible. Elle ne provient PAS de la simulation. Seule la position
   * de l'amas est physique : elle vient des cubes d'air que
   * Ciel::Nuageux() a marqués comme condensés.
   */
  static double formeNuage(double i, double j);

  /// Texture du relief selon l'altitude : gazon, roche, puis neige.
  QOpenGLTexture* textureDuRelief(double altitude) const;

  /// Dessine un quadrilatère texturé de coin (i, j) et d'altitudes données.
  void dessineTuile(QOpenGLTexture* texture, double i, double j,
                    double z00, double z10, double z11, double z01);

  /// Dessine une nappe nuageuse centrée sur le point de vue donné.
  void dessineAmasNuageux(QMatrix4x4 const& point_de_vue);

  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;

  // Textures (QOpenGLTexture remplace QGLContext::bindTexture, retiré en Qt 6)
  QOpenGLTexture* tex_nuages = nullptr;
  QOpenGLTexture* tex_roche  = nullptr;
  QOpenGLTexture* tex_gazon  = nullptr;
  QOpenGLTexture* tex_neige  = nullptr;

  //Les dimensions du dessin
  std::array<int, 3> Cubes = {0, 0, 0};

  // Caméra
  QMatrix4x4 matrice_vue;
};
