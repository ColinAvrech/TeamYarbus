#version 430

varying vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;

void main ()
{
  vec4 col = texture (image, Texcoord);// * vec4 (1.0, 1.0, 1.0, 1.0);
  float gray = 0.2126*col.r + 0.7152*col.g + 0.0722*col.b;
  //outColor = texture (image, Texcoord) * vec4 (1.0, 1.0, 1.0, 1.0);
  outColor = vec4(gray, gray, gray, col.a);
}