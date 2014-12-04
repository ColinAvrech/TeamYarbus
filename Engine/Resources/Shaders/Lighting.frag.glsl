#version 430

in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 Texcoord;

// OUTPUT COLOR AFTER ALL LIGHTING CALCULATIONS
out vec4 outColor;
// TEXTURE TO BE MAPPED ON QUAD
uniform sampler2D image;
uniform int enabled;

////////////////////////////////////////////
////////////////////////////////////////////
// LIGHT POSITION
uniform vec3 lightPos = vec3 (0, 0, 0);
uniform vec3 lights [5];
////////////////////////////////////////////
////////////////////////////////////////////

// MATERIAL PROPERTIES OF SELF
uniform vec3 mambient = vec3 (0.5, 0.5, 0.5);
uniform vec3 mdiffuse = vec3 (0.6, 0.6, 0.6);
uniform vec3 mspecular = vec3 (1, 1, 1);

////////////////////////////////////////////
////////////////////////////////////////////
// SHININESS
uniform float shininess = 4;
////////////////////////////////////////////
////////////////////////////////////////////

// MATERIAL PROPERTIES OF LIGHT
uniform vec3 lambient = vec3 (0.5, 0.5, 0.5);
uniform vec3 ldiffuse = vec3 (0.6, 0.6, 0.6);
uniform vec3 lspecular = vec3 (1, 1, 1);
uniform vec4 color;


void main()
{
  vec4 lightColor = vec4 (0,0,0,0);
  //distance from light-source to surface
  float dist = length (Position - lightPos);

  // calculate attentuation using distance from light
  float att = 1.5 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

  //the ambient light
  vec3 ambient = mambient * lambient;

  // calculate diffuse color
  vec3 surf2light = normalize (lightPos - Position);
  vec3 norm = normalize (Normal);
  float dcont = max (0.0, dot (norm, surf2light));
  vec3 diffuse = dcont * (mdiffuse * ldiffuse);

  // calculate specular color
  vec3 surf2view = normalize (-Position);
  vec3 reflection = reflect (-surf2light, norm);

  float scont = pow (max (0.0, dot(surf2view, reflection)), shininess);
  vec3 specular = scont * lspecular * mspecular;

  lightColor += vec4 ((ambient + diffuse + specular) * att, 1.0);
  // calculate resulting color
  outColor = texture (image, Texcoord) * lightColor;
}