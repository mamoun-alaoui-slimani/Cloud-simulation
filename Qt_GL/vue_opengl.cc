#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attr
#include <iostream>
#include "glsphere.h"
#include <cmath>
#include <QGLContext>
#include <QOpenGLFunctions>


/**
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue,
                               double rouge, double vert, double bleu)
{
  GLSphere sphere;
  sphere.initialize();
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  //prog.setAttributeValue(CouleurId, rouge, vert, bleu);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}

VueOpenGL::~VueOpenGL(){
    QGLContext* context =  const_cast<QGLContext*>(QGLContext::currentContext());

      context->deleteTexture(nuages);
      context->deleteTexture(montagne);
}
**/
VueOpenGL::~VueOpenGL()
{
  // Libère la mémoire des textures

  QGLContext* context =  const_cast<QGLContext*>(QGLContext::currentContext());

  context->deleteTexture(nuages);
  context->deleteTexture(montagne);
  context->deleteTexture(gazon);
  context->deleteTexture(neige);
}

void VueOpenGL::dessine(Ciel const& ciel)
{


    Cubes = ciel.getCubes();



    double eps(0.3);
    for(int i(0); i < Cubes[0]; ++i){
        for(int j(0); j < Cubes[1]; ++j){
            for(int k(0); k < Cubes[2]; ++k){
                if(not(i == 0 or j == 0 or i == Cubes[0] - 1 or j == Cubes[1] - 1 or k == 0 or k == Cubes[2]-1) and not (ciel.SousMontagne(i, j, k))){

                    if(ciel.Nuageux(i, j, k)){

                        QMatrix4x4 point_de_vue;
                        point_de_vue.translate(i,j,k+0.1);
                        point_de_vue.scale(0.6);
                        dessineMont(point_de_vue);
                        point_de_vue.translate(0,0,2);
                        point_de_vue.scale(-1);
                        dessineMont(point_de_vue);
                        point_de_vue.setToIdentity();

                        /**
                        point_de_vue.translate(i,j,k);
                        point_de_vue.scale(0.4);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();
**/
                        /**
                        point_de_vue.translate(i, j, k - eps);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();


                        point_de_vue.translate(i, j, k + eps);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();


                        point_de_vue.translate(i -eps, j, k);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();


                        point_de_vue.translate(i+eps, j, k);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();


                        point_de_vue.translate(i, j+eps, k);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                         point_de_vue.setToIdentity();

                        point_de_vue.translate(i, j -eps, k);
                        point_de_vue.scale(0.1);
                        dessineCube(point_de_vue);
                        point_de_vue.setToIdentity();
**/


/**
                        matrice.translate(i, j, k + eps);
                        matrice.scale(0.7);
                        dessineSphere(matrice, 0.5, 0.5, 0.5);
                        matrice.setToIdentity();
                        **/

/**
                        matrice.translate(i, j, k - eps);
                        matrice.scale(0.6);
                        dessineSphere(matrice, 0.5, 0.5, 0.5);
                        matrice.setToIdentity();


                            for(double phi(0.0); phi <= M_PI; phi += M_PI / 2){
                                matrice.translate(i + eps*cos(phi) ,j + eps*sin(phi),k);
                                matrice.scale(0.3);
                                dessineSphere(matrice, 0.5, 0.5, 0.5);
                                matrice.setToIdentity();
**/

                       }

                    }

                }
            }
        }
    }

void VueOpenGL::dessineMont(QMatrix4x4 const& point_de_vue){
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
    prog.setUniformValue("textureId", 0);

    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nuages);

    glBegin(GL_QUADS);
    double j(-2.0);
    while(j<= 2.0){
        for(double i(-2.0); i <= 2.0; i+=0.2){
            if(i*i + j*j <= 4){
                prog.setAttributeValue(CoordonnesTexture, 0.0, 0.0);
                prog.setAttributeValue(SommetId, i, j, altitude(i,j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 0.0);
                prog.setAttributeValue(SommetId, i + 1, j, altitude(i+1, j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 1.0);
                prog.setAttributeValue(SommetId, i + 1, j + 1, altitude(i+1, j+1));
                prog.setAttributeValue(CoordonnesTexture, 0.0, 1.0);
                prog.setAttributeValue(SommetId, i, j + 1, altitude(i, j+1));
                }
            }

        j+=0.2;
    }
        glEnd();


    }




double VueOpenGL::altitude(double i, double j) const
{

    double H =2* cos(j)+sin(i+1.5);
    return H;
  }

void VueOpenGL::dessine(Montagne const& montagne){



    /**
    //Dessin des axes pour aider à l'initialisation de la position
    QMatrix4x4 matrice;
    matrice.translate(0.0,0.0,0.0); //Origine en (0,0,0)
    matrice.scale(20); //Agrandissement des axes
    dessineAxes(matrice, true); //Dessin
    **/

    prog.setUniformValue("vue_modele", matrice_vue * QMatrix4x4());
    prog.setUniformValue("textureId", 0);
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();




    double j(0.0);
    while(j< Cubes[1]){
        for(double i(0.0); i < Cubes[0]; i++){

            if(montagne.altitude(i,j) < 3.0){

                glFuncs->glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->gazon);

                 glBegin(GL_QUADS);
                prog.setAttributeValue(CoordonnesTexture, 0.0, 0.0);
                prog.setAttributeValue(SommetId, i, j, montagne.altitude(i,j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 0.0);
                prog.setAttributeValue(SommetId, i + 1, j, montagne.altitude(i+1, j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 1.0);
                prog.setAttributeValue(SommetId, i + 1, j + 1, montagne.altitude(i+1, j+1));
                prog.setAttributeValue(CoordonnesTexture, 0.0, 1.0);
                prog.setAttributeValue(SommetId, i, j + 1, montagne.altitude(i, j+1));
                glEnd();

            }else if(montagne.altitude(i,j) < 10.0){

                 glFuncs->glActiveTexture(GL_TEXTURE0);
                 glBindTexture(GL_TEXTURE_2D, this->montagne);
                glBegin(GL_QUADS);
                prog.setAttributeValue(CoordonnesTexture, 0.0, 0.0);
                prog.setAttributeValue(SommetId, i, j, montagne.altitude(i,j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 0.0);
                prog.setAttributeValue(SommetId, i + 1, j, montagne.altitude(i+1, j));
                prog.setAttributeValue(CoordonnesTexture, 1.0, 1.0);
                prog.setAttributeValue(SommetId, i + 1, j + 1, montagne.altitude(i+1, j+1));
                prog.setAttributeValue(CoordonnesTexture, 0.0, 1.0);
                prog.setAttributeValue(SommetId, i, j + 1, montagne.altitude(i, j+1));
                glEnd();
            }else{
                glFuncs->glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->neige);
               glBegin(GL_QUADS);
               prog.setAttributeValue(CoordonnesTexture, 0.0, 0.0);
               prog.setAttributeValue(SommetId, i, j, montagne.altitude(i,j));
               prog.setAttributeValue(CoordonnesTexture, 1.0, 0.0);
               prog.setAttributeValue(SommetId, i + 1, j, montagne.altitude(i+1, j));
               prog.setAttributeValue(CoordonnesTexture, 1.0, 1.0);
               prog.setAttributeValue(SommetId, i + 1, j + 1, montagne.altitude(i+1, j+1));
               prog.setAttributeValue(CoordonnesTexture, 0.0, 1.0);
               prog.setAttributeValue(SommetId, i, j + 1, montagne.altitude(i, j+1));
               glEnd();
            }

            }
        j++;
    }


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


  // Préparation d'une première texture à partir d'une image (cat.jpeg).
   QGLContext* context =  const_cast<QGLContext*>(QGLContext::currentContext());
   nuages = context->bindTexture(QPixmap(":/nuages.jpeg"), GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
   // Autres variantes au lieu de GL_MIRRORED_REPEAT : GL_REPEAT, GL_CLAMP_TO_EDGE

   // Préparation d'une seconde texture.
   // S'il y devait y en avoir plus, on ferait bien sûr une fonction ;-)
   montagne = context->bindTexture(QPixmap(":/montagne.jpeg"), GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


   gazon = context->bindTexture(QPixmap(":/gazon.jpeg"), GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


   neige = context->bindTexture(QPixmap(":/neige.jpeg"), GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);



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


void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}
