#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QImage>
#include <QtGlobal>
#include <cmath>

// ======================================================================
VueOpenGL::~VueOpenGL()
{
  // Libère la mémoire des textures
  delete tex_nuages;
  delete tex_roche;
  delete tex_gazon;
  delete tex_neige;
}

// ======================================================================
void VueOpenGL::dessine(Ciel const& ciel)
{
  prog.bind(); // Qt 6 délie le programme entre les frames

  Cubes = ciel.getCubes();

  for(int i(0); i < Cubes[0]; ++i){
    for(int j(0); j < Cubes[1]; ++j){
      for(int k(0); k < Cubes[2]; ++k){

        // On ignore les bords de la boîte et les cubes enterrés dans le relief
        bool const bord(i == 0 or j == 0 or k == 0
                        or i == Cubes[0] - 1 or j == Cubes[1] - 1 or k == Cubes[2] - 1);
        if(bord or ciel.SousMontagne(i, j, k) or not ciel.Nuageux(i, j, k)) continue;

        /* Un amas est fait de deux nappes opposées : la seconde est la
           première retournée, ce qui referme le nuage sur lui-même. */
        QMatrix4x4 point_de_vue;
        point_de_vue.translate(i, j, k + 0.1);
        point_de_vue.scale(0.6);
        dessineAmasNuageux(point_de_vue);
        point_de_vue.translate(0, 0, 2);
        point_de_vue.scale(-1);
        dessineAmasNuageux(point_de_vue);
      }
    }
  }
}

// ======================================================================
void VueOpenGL::dessine(Montagne const& montagne)
{
  prog.bind(); // Qt 6 délie le programme entre les frames

  prog.setUniformValue("vue_modele", matrice_vue * QMatrix4x4());
  prog.setUniformValue("textureId", 0);

  for(double j(0.0); j < Cubes[1]; ++j){
    for(double i(0.0); i < Cubes[0]; ++i){
      double const z00(montagne.altitude(i, j));
      dessineTuile(textureDuRelief(z00), i, j,
                   z00,
                   montagne.altitude(i + 1, j),
                   montagne.altitude(i + 1, j + 1),
                   montagne.altitude(i, j + 1));
    }
  }
}

// ======================================================================
void VueOpenGL::dessineAmasNuageux(QMatrix4x4 const& point_de_vue)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setUniformValue("textureId", 0);

  for(double j(-RAYON_NUAGE); j <= RAYON_NUAGE; j += PAS_NUAGE){
    for(double i(-RAYON_NUAGE); i <= RAYON_NUAGE; i += PAS_NUAGE){
      if(i*i + j*j > RAYON_NUAGE * RAYON_NUAGE) continue;
      dessineTuile(tex_nuages, i, j,
                   formeNuage(i, j),
                   formeNuage(i + 1, j),
                   formeNuage(i + 1, j + 1),
                   formeNuage(i, j + 1));
    }
  }
}

// ======================================================================
void VueOpenGL::dessineTuile(QOpenGLTexture* texture, double i, double j,
                             double z00, double z10, double z11, double z01)
{
  texture->bind(0);

  glBegin(GL_QUADS);
  prog.setAttributeValue(CoordonnesTexture, 0.0, 0.0);
  prog.setAttributeValue(SommetId, i,     j,     z00);
  prog.setAttributeValue(CoordonnesTexture, 1.0, 0.0);
  prog.setAttributeValue(SommetId, i + 1, j,     z10);
  prog.setAttributeValue(CoordonnesTexture, 1.0, 1.0);
  prog.setAttributeValue(SommetId, i + 1, j + 1, z11);
  prog.setAttributeValue(CoordonnesTexture, 0.0, 1.0);
  prog.setAttributeValue(SommetId, i,     j + 1, z01);
  glEnd();
}

// ======================================================================
QOpenGLTexture* VueOpenGL::textureDuRelief(double altitude) const
{
  if(altitude < ALTITUDE_GAZON) return tex_gazon;
  if(altitude < ALTITUDE_ROCHE) return tex_roche;
  return tex_neige;
}

// ======================================================================
double VueOpenGL::formeNuage(double i, double j)
{
  return 2.0 * std::cos(j) + std::sin(i + 1.5);
}

// ======================================================================
QOpenGLTexture* VueOpenGL::chargeTexture(QString const& chemin)
{
  // Autres variantes au lieu de MirroredRepeat : Repeat, ClampToEdge
  /* Le retournement vertical reproduit ce que QGLContext::bindTexture
     faisait implicitement. flipped() remplace mirrored() depuis Qt 6.9. */
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
  QImage const image(QImage(chemin).flipped(Qt::Vertical));
#else
  QImage const image(QImage(chemin).mirrored());
#endif
  QOpenGLTexture* texture(new QOpenGLTexture(image));
  texture->setWrapMode(QOpenGLTexture::MirroredRepeat);
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  return texture;
}

// ======================================================================
void VueOpenGL::init()
{
  initializeOpenGLFunctions();

  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("coordonnee_texture", CoordonnesTexture);

  // Activation du shader
  prog.bind();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  /* Préparation des textures. QGLContext::bindTexture n'existe plus
     en Qt 6 : on passe par QOpenGLTexture. */
  tex_nuages = chargeTexture(":/nuages.jpeg");
  tex_roche  = chargeTexture(":/montagne.jpeg");
  tex_gazon  = chargeTexture(":/gazon.jpeg");
  tex_neige  = chargeTexture(":/neige.jpeg");

  initializePosition();
}

// ======================================================================
void VueOpenGL::initializePosition()
{
  // position initiale
  matrice_vue.setToIdentity();
  matrice_vue.translate(0.0, 0.0, -4.0);
  matrice_vue.rotate(-90.0, 1.0, 0.0, 0.0); //on regarde depuis l'axe des x
  matrice_vue.rotate(45, 0.0, 0.0, 1.0); //Vue de côté
  matrice_vue.translate(10.0, 10.0, -10.0); //Prise de distance
}

// ======================================================================
void VueOpenGL::translate(double x, double y, double z)
{
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(x, y, z);
  matrice_vue = translation_supplementaire * matrice_vue;
}

// ======================================================================
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}
