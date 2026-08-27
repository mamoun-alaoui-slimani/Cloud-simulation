#pragma once

#include <QOpenGLShaderProgram> // Qt wrapper around the GLSL program
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QString>
#include <vector>
#include "Renderer.h"
#include "System.h"

/**
 * @brief Real-time 3D view of the system, in OpenGL.
 *
 * Implements Renderer: the physics model knows nothing about this
 * class, it only knows how to ask a renderer to draw it.
 *
 * The terrain and the cloud puff are fixed geometry, so each is uploaded
 * to a vertex buffer once and then redrawn from there. A puff is the same
 * mesh every time; only its model matrix changes.
 */
class OpenGLRenderer : public Renderer, protected QOpenGLFunctions {
public:
    ~OpenGLRenderer() override; // frees the textures and buffers

    // drawing methods inherited from Renderer
    void draw(Mountain const& mountain) override;
    void draw(Sky const& sky) override;

    // (re-)initialisation
    void init();
    void initializePosition();

    void setProjection(QMatrix4x4 const& projection) {
        program.bind();
        program.setUniformValue("projection", projection);
    }

    // camera movement
    void translate(double x, double y, double z);
    void rotate(double angle, double dir_x, double dir_y, double dir_z);

private:
    // Altitudes at which the terrain texture changes
    static constexpr double GRASS_ALTITUDE = 3.0;
    static constexpr double ROCK_ALTITUDE = 10.0;

    // Extent and resolution of the sheet drawn for one cloud puff
    static constexpr double CLOUD_RADIUS = 2.0;
    static constexpr double CLOUD_STEP = 0.2;

    // Floats per vertex: three of position, two of texture coordinate
    static constexpr int FLOATS_PER_VERTEX = 5;

    /// A chunk of static geometry living in a vertex buffer.
    struct Mesh {
        QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
        int vertexCount = 0;
        bool isEmpty() const { return vertexCount == 0; }
    };

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

    /// Appends one quad, as two triangles, to a vertex array being built.
    static void appendTile(std::vector<GLfloat>& data, double i, double j, double z00, double z10,
                           double z11, double z01);

    /// Uploads a vertex array to the GPU.
    void uploadMesh(Mesh& mesh, std::vector<GLfloat> const& data);

    /// Draws a mesh with the given texture.
    void drawMesh(Mesh& mesh, QOpenGLTexture* texture);

    /// Builds the three terrain meshes, one per texture. Called once.
    void buildTerrainMeshes(Mountain const& mountain);

    /// Builds the cloud puff mesh. Called once.
    void buildCloudMesh();

    // The GLSL program, wrapped by Qt
    QOpenGLShaderProgram program;

    // Textures (QOpenGLTexture replaces QGLContext::bindTexture, removed in Qt 6)
    QOpenGLTexture* cloudTexture = nullptr;
    QOpenGLTexture* rockTexture = nullptr;
    QOpenGLTexture* grassTexture = nullptr;
    QOpenGLTexture* snowTexture = nullptr;

    // Static geometry, uploaded once
    Mesh grassMesh;
    Mesh rockMesh;
    Mesh snowMesh;
    Mesh cloudMesh;
    bool terrainBuilt = false;

    // Grid dimensions being drawn
    std::array<int, 3> cellCount = {0, 0, 0};

    // Camera
    QMatrix4x4 viewMatrix;
};
