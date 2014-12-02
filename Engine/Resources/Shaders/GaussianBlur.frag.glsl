#version 430

uniform sampler2D image; // this should hold the texture rendered by the horizontal blur pass
in vec2 Texcoord;
 
const float blurSize = 1.0/1024.0;
 
out vec4 outColor;

void main(void)
{
   vec4 sum = vec4(0.0);
 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y - 4.0*blurSize)) * 0.05;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y - 3.0*blurSize)) * 0.09;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y - 2.0*blurSize)) * 0.12;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y - blurSize))     * 0.15;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y))                * 0.16;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y + blurSize))     * 0.15;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y + 2.0*blurSize)) * 0.12;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y + 3.0*blurSize)) * 0.09;
   sum += texture2D(image, vec2(Texcoord.x, Texcoord.y + 4.0*blurSize)) * 0.05;
 
   outColor = sum;
}