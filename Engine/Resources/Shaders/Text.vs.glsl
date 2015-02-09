#version 430

in vec4 position;

out vec2 Texcoord;

uniform mat4 mvp;


void main()
{
  gl_Position = mvp * vec4(position.xy, 1, 1);
  Texcoord = position.zw;
}