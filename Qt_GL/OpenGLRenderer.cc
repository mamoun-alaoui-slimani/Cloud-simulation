#include "OpenGLRenderer.h"
#include "ShaderAttributes.h" // Qt ids of the shader attributes
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QImage>
#include <QtGlobal>
#include <cmath>

// ======================================================================
OpenGLRenderer::~OpenGLRenderer()
{
  // Frees the textures
  delete cloudTexture;
  delete rockTexture;
  delete grassTexture;
  delete snowTexture;
}

// ======================================================================
void OpenGLRenderer::draw(Sky const& sky)
{
  program.bind(); // Qt 6 unbinds the program between frames

  cellCount = sky.getCellCount();

  for(int i(0); i < cellCount[0]; ++i){
    for(int j(0); j < cellCount[1]; ++j){
      for(int k(0); k < cellCount[2]; ++k){

        // Skip the edges of the box and the cells buried in the terrain
        bool const bord(i == 0 or j == 0 or k == 0
                        or i == cellCount[0] - 1 or j == cellCount[1] - 1 or k == cellCount[2] - 1);
        if(bord or sky.isBelowTerrain(i, j, k) or not sky.isCloudy(i, j, k)) continue;

        /* A puff is two opposing sheets: the second is the first
           flipped, which closes the cloud on itself. */
        QMatrix4x4 modelMatrix;
        modelMatrix.translate(i, j, k + 0.1);
        modelMatrix.scale(0.6);
        drawCloudPuff(modelMatrix);
        modelMatrix.translate(0, 0, 2);
        modelMatrix.scale(-1);
        drawCloudPuff(modelMatrix);
      }
    }
  }
}

// ======================================================================
void OpenGLRenderer::draw(Mountain const& mountain)
{
  program.bind(); // Qt 6 unbinds the program between frames

  program.setUniformValue("modelView", viewMatrix * QMatrix4x4());
  program.setUniformValue("textureId", 0);

  for(double j(0.0); j < cellCount[1]; ++j){
    for(double i(0.0); i < cellCount[0]; ++i){
      double const z00(mountain.altitude(i, j));
      drawTile(terrainTexture(z00), i, j,
                   z00,
                   mountain.altitude(i + 1, j),
                   mountain.altitude(i + 1, j + 1),
                   mountain.altitude(i, j + 1));
    }
  }
}

// ======================================================================
void OpenGLRenderer::drawCloudPuff(QMatrix4x4 const& modelMatrix)
{
  program.setUniformValue("modelView", viewMatrix * modelMatrix);
  program.setUniformValue("textureId", 0);

  for(double j(-CLOUD_RADIUS); j <= CLOUD_RADIUS; j += CLOUD_STEP){
    for(double i(-CLOUD_RADIUS); i <= CLOUD_RADIUS; i += CLOUD_STEP){
      if(i*i + j*j > CLOUD_RADIUS * CLOUD_RADIUS) continue;
      drawTile(cloudTexture, i, j,
                   cloudSurface(i, j),
                   cloudSurface(i + 1, j),
                   cloudSurface(i + 1, j + 1),
                   cloudSurface(i, j + 1));
    }
  }
}

// ======================================================================
void OpenGLRenderer::drawTile(QOpenGLTexture* texture, double i, double j,
                             double z00, double z10, double z11, double z01)
{
  texture->bind(0);

  glBegin(GL_QUADS);
  program.setAttributeValue(TextureCoordId, 0.0, 0.0);
  program.setAttributeValue(VertexId, i,     j,     z00);
  program.setAttributeValue(TextureCoordId, 1.0, 0.0);
  program.setAttributeValue(VertexId, i + 1, j,     z10);
  program.setAttributeValue(TextureCoordId, 1.0, 1.0);
  program.setAttributeValue(VertexId, i + 1, j + 1, z11);
  program.setAttributeValue(TextureCoordId, 0.0, 1.0);
  program.setAttributeValue(VertexId, i,     j + 1, z01);
  glEnd();
}

// ======================================================================
QOpenGLTexture* OpenGLRenderer::terrainTexture(double altitude) const
{
  if(altitude < GRASS_ALTITUDE) return grassTexture;
  if(altitude < ROCK_ALTITUDE) return rockTexture;
  return snowTexture;
}

// ======================================================================
double OpenGLRenderer::cloudSurface(double i, double j)
{
  return 2.0 * std::cos(j) + std::sin(i + 1.5);
}

// ======================================================================
QOpenGLTexture* OpenGLRenderer::loadTexture(QString const& path)
{
  // Autres variantes au lieu de MirroredRepeat : Repeat, ClampToEdge
  /* The vertical flip reproduces what QGLContext::bindTexture did
     implicitly. flipped() replaces mirrored() as of Qt 6.9. */
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
  QImage const image(QImage(path).flipped(Qt::Vertical));
#else
  QImage const image(QImage(path).mirrored());
#endif
  QOpenGLTexture* texture(new QOpenGLTexture(image));
  texture->setWrapMode(QOpenGLTexture::MirroredRepeat);
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  return texture;
}

// ======================================================================
void OpenGLRenderer::init()
{
  initializeOpenGLFunctions();

  program.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

  program.bindAttributeLocation("vertex",  VertexId);
  program.bindAttributeLocation("textureCoord", TextureCoordId);

  // Activation du shader
  program.bind();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  /* QGLContext::bindTexture no longer exists in Qt 6, so the textures
     go through QOpenGLTexture. */
  cloudTexture = loadTexture(":/clouds.jpeg");
  rockTexture  = loadTexture(":/rock.jpeg");
  grassTexture  = loadTexture(":/grass.jpeg");
  snowTexture  = loadTexture(":/snow.jpeg");

  initializePosition();
}

// ======================================================================
void OpenGLRenderer::initializePosition()
{
  // position initiale
  viewMatrix.setToIdentity();
  viewMatrix.translate(0.0, 0.0, -4.0);
  viewMatrix.rotate(-90.0, 1.0, 0.0, 0.0); //look along the x axis
  viewMatrix.rotate(45, 0.0, 0.0, 1.0); //side view
  viewMatrix.translate(10.0, 10.0, -10.0); //Prise de distance
}

// ======================================================================
void OpenGLRenderer::translate(double x, double y, double z)
{
  /* Multiplies the view matrix FROM THE LEFT, so that the most recent
   * change is applied last, the way function composition works.
   */
  QMatrix4x4 extraTranslation;
  extraTranslation.translate(x, y, z);
  viewMatrix = extraTranslation * viewMatrix;
}

// ======================================================================
void OpenGLRenderer::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplies the view matrix FROM THE LEFT
  QMatrix4x4 extraRotation;
  extraRotation.rotate(angle, dir_x, dir_y, dir_z);
  viewMatrix = extraRotation * viewMatrix;
}
