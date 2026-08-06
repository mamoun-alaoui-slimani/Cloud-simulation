#pragma once

#include <QGLBuffer>
#include <QOpenGLShaderProgram>

class GLSphere
{
public:
 GLSphere()
   : vbo(QGLBuffer::VertexBuffer), ibo(QGLBuffer::IndexBuffer)
 {}

  void initialize(GLuint slices = 10, GLuint stacks = 10);

  void draw(QOpenGLShaderProgram& program, int attributeLocation);

  void bind();
  void release();

private:
  QGLBuffer vbo, ibo;
  GLuint vbo_sz;
  GLuint ibo_sz[3];
};
