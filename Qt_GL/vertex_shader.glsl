attribute vec3 vertex;
attribute vec2 textureCoord;

uniform mat4 projection;
uniform mat4 modelView;

varying vec2 fragTextureCoord;

void main()
{
  gl_Position = projection * modelView * vec4(vertex, 1.0);
  fragTextureCoord = textureCoord;
}
