#version 430

in vec2 Position;
in vec2 Texcoord;

uniform vec2 lightPosition;
uniform vec4 lightColor;
uniform float lightRadius;
uniform float intensity;
uniform float screenHeight;
uniform sampler2D image;

out vec4 outColor;

void main()
{
  vec4 Color = vec4 (0,0,0,0);
  for (int i = 0; i < 1; ++i)
  {
    float distance = length(lightPosition - Position);
    float attenuation = intensity / (1.0 + 10.0 * (distance / lightRadius));
	  //float attenuation = 1.0 / distance;
	  vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 3)) * lightColor;
    //color += vec4 (0.5, 0.5, 0.5, 0.5);
	  Color += texture (image, Texcoord) * color;
  }

  outColor = Color;
  //vec2 pixel=gl_FragCoord.xy;	
  //pixel.y=screenHeight-pixel.y;	
  //vec2 aux = lightLocation - pixel;
  //float distance = length(aux);
  //float attenuation=1.0/(0 * distance + 0.5 *distance*distance);	
  //vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(lightColor,1.0);	
  //outColor = color;//*texture2D(texture,gl_TexCoord[0].st);
}