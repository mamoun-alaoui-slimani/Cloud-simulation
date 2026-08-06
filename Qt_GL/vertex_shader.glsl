attribute vec3 sommet;
attribute vec2 coordonnee_texture;

uniform mat4 projection;
uniform mat4 vue_modele;

varying vec2 ma_coordonnee_texture;

void main()
{
  gl_Position = projection * vue_modele * vec4(sommet, 1.0);
  ma_coordonnee_texture = coordonnee_texture;
}
