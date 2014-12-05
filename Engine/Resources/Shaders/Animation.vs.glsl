#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec2 Texcoord;
out vec3 Normal;

uniform mat4 mvp;
uniform vec2 texOffset;
uniform vec2 frameRatio;

void main()
{
  gl_Position = mvp * vec4(position, 1.0);
  Color = color;
  Texcoord = vec2 (texOffset.x + texcoord.x * frameRatio.x, texOffset.y + texcoord.y * frameRatio.y);
  Position = position;
  Normal = normal;
}