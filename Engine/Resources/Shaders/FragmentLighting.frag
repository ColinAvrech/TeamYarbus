#version 430

in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 Texcoord;

// OUTPUT COLOR AFTER ALL LIGHTING CALCULATIONS
out vec4 outColor;
// TEXTURE TO BE MAPPED ON QUAD
uniform sampler2D image;

uniform vec3 lightPos;

// MATERIAL PROPERTIES OF SELF
uniform vec3 mambient;
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform float shininess;

// MATERIAL PROPERTIES OF LIGHT
uniform vec3 lambient;
uniform vec3 ldiffuse;
uniform vec3 lspecular;


void main()
{
  //distance from light-source to surface
  float dist = length (Position - lightPos);

  // calculate attentuation using distance from light
  float att = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

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

  // calculate resulting color
  outColor = texture (image, Texcoord) * vec4 ((ambient + diffuse + specular) * att, 1.0);
}