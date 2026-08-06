uniform sampler2D textureId;

varying vec2 ma_coordonnee_texture;

void main()
{
  gl_FragColor = texture2D(textureId, ma_coordonnee_texture);
}
