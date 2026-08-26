uniform sampler2D textureId;

varying vec2 fragTextureCoord;

void main()
{
  gl_FragColor = texture2D(textureId, fragTextureCoord);
}
