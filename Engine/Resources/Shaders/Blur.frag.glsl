#version 430

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D image;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;

void main ()
{
  vec4 sum = vec4(0.0);
  for (int x = -4; x <= 4; x++)
  {
    for (int y = -4; y <= 4; y++)
    {
      sum += texture(image, vec2(Texcoord.x + x * blurSizeH, Texcoord.y + y * blurSizeV)) / 81.0;
    }
  }
  outColor = sum;
}