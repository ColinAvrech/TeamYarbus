#version 430


in vec3 Normal;
in vec3 Position;
uniform mat4 modelMatrix;

out vec4 outColor;
////////////////////////////////////////////
////////////////////////////////////////////
// LIGHT POSITION
uniform float time;// = vec3 (0, 47, 0);
uniform vec3 lights [5];
////////////////////////////////////////////
////////////////////////////////////////////

// MATERIAL PROPERTIES OF SELF
uniform vec3 mambient = vec3 (0.2, 0.1, 0.2);
uniform vec3 mdiffuse = vec3 (0.6, 0.3, 0.0);
uniform vec3 mspecular = vec3 (1, 1, 1);

////////////////////////////////////////////
////////////////////////////////////////////
// SHININESS
uniform float shininess = 5;
////////////////////////////////////////////
////////////////////////////////////////////

// MATERIAL PROPERTIES OF LIGHT
uniform vec3 lambient = vec3 (0.2, 0.0, 0.1);
uniform vec3 ldiffuse = vec3 (0.6, 0.6, 0.0);
uniform vec3 lspecular = vec3 (1, 1, 1);
uniform vec4 color;

void main ()
{
  //Only for testing with a moving light
  vec3 lightPos = vec3(17, 15.7, 0);//vec3(20 * cos(time / 10.f), 45 + 7 * sin(time / 10.f), 7 * sin(time / 5.f)); 
  vec4 m_pos = modelMatrix * vec4(Position, 1.0);
  vec3 position = vec3(m_pos.x, m_pos.y, m_pos.z);
  vec4 lightColor = vec4 (0,0,0,0);
  //distance from light-source to surface
  float dist = length (position - lightPos);

  // calculate attentuation using distance from light
  float att = 1.5 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

  //the ambient light
  vec3 ambient = mambient * lambient;

  // calculate diffuse color
  vec3 surf2light = normalize (lightPos - position);
  vec3 norm = normalize (-Normal);
  float dcont = max (0.0, dot (norm, surf2light));
  vec3 diffuse = dcont * (mdiffuse * ldiffuse);

  // calculate specular color
  vec3 surf2view = normalize (-position);
  vec3 reflection = reflect (-surf2light, norm);

  float scont = pow (max (0.0, dot(surf2view, reflection)), shininess);
  vec3 specular = scont * lspecular * mspecular;

  lightColor += vec4 ((ambient + diffuse + specular) * att, 1.0);
  // calculate resulting color
  //outColor = vec4(Normal.r * 0.5 + 0.5, Normal.g * 0.5 + 0.5, Normal.b * 0.5 + 0.5, 1.0);
  outColor = vec4(color.r * lightColor.r, color.g * lightColor.g, color.b * lightColor.b, 1);
}