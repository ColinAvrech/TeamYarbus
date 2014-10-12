#version 430

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texcoord;

out vec3 Position;
out vec4 Color;
out vec2 Texcoord;
out vec3 Normal;

uniform mat4 modelViewProjectionMatrix;
uniform vec2 texOffset;
uniform vec2 frameRatio;

void main()
{
  gl_Position = modelViewProjectionMatrix * vec4(position, 0.5);
  Color = color;
  Texcoord = vec2 (texOffset.x + texcoord.x * frameRatio.x, texOffset.y + texcoord.y * frameRatio.y);
  Position = vec3 (position.x, position.y, 1.0);
  Normal = normal;
}