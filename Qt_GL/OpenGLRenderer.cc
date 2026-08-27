#include "OpenGLRenderer.h"
#include "ShaderAttributes.h" // Qt ids of the shader attributes
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QImage>
#include <QtGlobal>
#include <cmath>

// ======================================================================
OpenGLRenderer::~OpenGLRenderer() {
    // Frees the textures
    delete cloudTexture;
    delete rockTexture;
    delete grassTexture;
    delete snowTexture;
}

// ======================================================================
void OpenGLRenderer::draw(Sky const& sky) {
    program.bind(); // Qt 6 unbinds the program between frames

    cellCount = sky.getCellCount();
    program.setUniformValue("textureId", 0);

    for (int i(0); i < cellCount[0]; ++i) {
        for (int j(0); j < cellCount[1]; ++j) {
            for (int k(0); k < cellCount[2]; ++k) {

                // Skip the edges of the box and the cells buried in the terrain
                bool const edge(i == 0 or j == 0 or k == 0 or i == cellCount[0] - 1 or
                                j == cellCount[1] - 1 or k == cellCount[2] - 1);
                if (edge or sky.isBelowTerrain(i, j, k) or not sky.isCloudy(i, j, k))
                    continue;

                /* A puff is two opposing sheets: the second is the first
                   flipped, which closes the cloud on itself. Both are the same
                   cached mesh, moved into place by the model matrix. */
                QMatrix4x4 modelMatrix;
                modelMatrix.translate(i, j, k + 0.1);
                modelMatrix.scale(0.6);
                program.setUniformValue("modelView", viewMatrix * modelMatrix);
                drawMesh(cloudMesh, cloudTexture);

                modelMatrix.translate(0, 0, 2);
                modelMatrix.scale(-1);
                program.setUniformValue("modelView", viewMatrix * modelMatrix);
                drawMesh(cloudMesh, cloudTexture);
            }
        }
    }
}

// ======================================================================
void OpenGLRenderer::draw(Mountain const& mountain) {
    program.bind(); // Qt 6 unbinds the program between frames

    // The terrain never changes, so it is uploaded once and then reused.
    if (not terrainBuilt)
        buildTerrainMeshes(mountain);

    program.setUniformValue("modelView", viewMatrix * QMatrix4x4());
    program.setUniformValue("textureId", 0);

    drawMesh(grassMesh, grassTexture);
    drawMesh(rockMesh, rockTexture);
    drawMesh(snowMesh, snowTexture);
}

// ======================================================================
void OpenGLRenderer::buildTerrainMeshes(Mountain const& mountain) {
    std::vector<GLfloat> grass, rock, snow;

    for (double j(0.0); j < cellCount[1]; ++j) {
        for (double i(0.0); i < cellCount[0]; ++i) {
            double const z00(mountain.altitude(i, j));

            std::vector<GLfloat>& target(z00 < GRASS_ALTITUDE  ? grass
                                         : z00 < ROCK_ALTITUDE ? rock
                                                               : snow);
            appendTile(target, i, j, z00, mountain.altitude(i + 1, j),
                       mountain.altitude(i + 1, j + 1), mountain.altitude(i, j + 1));
        }
    }

    uploadMesh(grassMesh, grass);
    uploadMesh(rockMesh, rock);
    uploadMesh(snowMesh, snow);
    terrainBuilt = true;
}

// ======================================================================
void OpenGLRenderer::buildCloudMesh() {
    /* Every puff is the same sheet, so this is built once. The tiles are
       one unit wide but only CLOUD_STEP apart, so they overlap heavily,
       which is what gives a puff its billowy look. */
    std::vector<GLfloat> data;

    for (double j(-CLOUD_RADIUS); j <= CLOUD_RADIUS; j += CLOUD_STEP) {
        for (double i(-CLOUD_RADIUS); i <= CLOUD_RADIUS; i += CLOUD_STEP) {
            if (i * i + j * j > CLOUD_RADIUS * CLOUD_RADIUS)
                continue;
            appendTile(data, i, j, cloudSurface(i, j), cloudSurface(i + 1, j),
                       cloudSurface(i + 1, j + 1), cloudSurface(i, j + 1));
        }
    }

    uploadMesh(cloudMesh, data);
}

// ======================================================================
void OpenGLRenderer::appendTile(std::vector<GLfloat>& data, double i, double j, double z00,
                                double z10, double z11, double z01) {
    // The quad (0,1,2,3) becomes the triangles (0,1,2) and (0,2,3),
    // keeping the winding the fixed-function version used.
    GLfloat const corners[4][5] = {
        {GLfloat(i), GLfloat(j), GLfloat(z00), 0.0f, 0.0f},
        {GLfloat(i + 1), GLfloat(j), GLfloat(z10), 1.0f, 0.0f},
        {GLfloat(i + 1), GLfloat(j + 1), GLfloat(z11), 1.0f, 1.0f},
        {GLfloat(i), GLfloat(j + 1), GLfloat(z01), 0.0f, 1.0f},
    };
    for (int index : {0, 1, 2, 0, 2, 3})
        for (int c(0); c < FLOATS_PER_VERTEX; ++c)
            data.push_back(corners[index][c]);
}

// ======================================================================
void OpenGLRenderer::uploadMesh(Mesh& mesh, std::vector<GLfloat> const& data) {
    mesh.vertexCount = int(data.size()) / FLOATS_PER_VERTEX;
    if (mesh.isEmpty())
        return;

    mesh.vbo.create();
    mesh.vbo.bind();
    mesh.vbo.allocate(data.data(), int(data.size() * sizeof(GLfloat)));
    mesh.vbo.release();
}

// ======================================================================
void OpenGLRenderer::drawMesh(Mesh& mesh, QOpenGLTexture* texture) {
    if (mesh.isEmpty())
        return;

    texture->bind(0);
    mesh.vbo.bind();

    constexpr int stride = FLOATS_PER_VERTEX * sizeof(GLfloat);
    program.setAttributeBuffer(VertexId, GL_FLOAT, 0, 3, stride);
    program.enableAttributeArray(VertexId);
    program.setAttributeBuffer(TextureCoordId, GL_FLOAT, 3 * sizeof(GLfloat), 2, stride);
    program.enableAttributeArray(TextureCoordId);

    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

    program.disableAttributeArray(VertexId);
    program.disableAttributeArray(TextureCoordId);
    mesh.vbo.release();
}

// ======================================================================
double OpenGLRenderer::cloudSurface(double i, double j) {
    return 2.0 * std::cos(j) + std::sin(i + 1.5);
}

// ======================================================================
QOpenGLTexture* OpenGLRenderer::loadTexture(QString const& path) {
    /* The vertical flip reproduces what QGLContext::bindTexture did
       implicitly. flipped() replaces mirrored() as of Qt 6.9. */
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    QImage const image(QImage(path).flipped(Qt::Vertical));
#else
    QImage const image(QImage(path).mirrored());
#endif
    QOpenGLTexture* texture(new QOpenGLTexture(image));
    // Other options instead of MirroredRepeat: Repeat, ClampToEdge
    texture->setWrapMode(QOpenGLTexture::MirroredRepeat);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    return texture;
}

// ======================================================================
void OpenGLRenderer::init() {
    initializeOpenGLFunctions();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

    program.bindAttributeLocation("vertex", VertexId);
    program.bindAttributeLocation("textureCoord", TextureCoordId);

    program.bind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /* QGLContext::bindTexture no longer exists in Qt 6, so the textures
       go through QOpenGLTexture. */
    cloudTexture = loadTexture(":/clouds.jpeg");
    rockTexture = loadTexture(":/rock.jpeg");
    grassTexture = loadTexture(":/grass.jpeg");
    snowTexture = loadTexture(":/snow.jpeg");

    buildCloudMesh();

    initializePosition();
}

// ======================================================================
void OpenGLRenderer::initializePosition() {
    // initial position
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0, 0.0, -4.0);
    viewMatrix.rotate(-90.0, 1.0, 0.0, 0.0); // look along the x axis
    viewMatrix.rotate(45, 0.0, 0.0, 1.0);    // side view
    viewMatrix.translate(10.0, 10.0, -10.0); // step back
}

// ======================================================================
void OpenGLRenderer::translate(double x, double y, double z) {
    /* Multiplies the view matrix FROM THE LEFT, so that the most recent
     * change is applied last, the way function composition works.
     */
    QMatrix4x4 extraTranslation;
    extraTranslation.translate(x, y, z);
    viewMatrix = extraTranslation * viewMatrix;
}

// ======================================================================
void OpenGLRenderer::rotate(double angle, double dir_x, double dir_y, double dir_z) {
    // Multiplies the view matrix FROM THE LEFT
    QMatrix4x4 extraRotation;
    extraRotation.rotate(angle, dir_x, dir_y, dir_z);
    viewMatrix = extraRotation * viewMatrix;
}
