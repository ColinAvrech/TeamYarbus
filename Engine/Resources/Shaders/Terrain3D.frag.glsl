#version 430

in vec3 Position;
in vec3 Normal;
in vec3 Vert_Pos;

uniform vec4 color1;
uniform vec4 color2;

// OUTPUT COLOR AFTER ALL LIGHTING CALCULATIONS
out vec4 outColor;

// LIGHT POSITION
uniform vec3 lightDir;// = vec3 (50, -10, 0);

// MATERIAL PROPERTIES OF SELF
vec3 mambient = vec3 (0.1, 0.1, 0.25);
vec3 mdiffuse = vec3 (0.5, 0.5, 0.5);
vec3 mspecular = vec3 (1, 1, 1);

// SHININESS
float shininess = 5;

// MATERIAL PROPERTIES OF LIGHT
uniform vec3 lambient = vec3 (0.1, 0.1, 0.2);
uniform vec3 ldiffuse = vec3 (0.5, 0.5, 0.5);
uniform vec3 lspecular = vec3 (1, 1, 1);

void main ()
{
  vec4 lightColor = vec4 (0,0,0,0);
  //distance from light-source to surface
  //float dist = length (lightDir);

  // calculate attentuation using distance from light
  //float att = 1.5 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

  //the ambient light
  vec3 ambient = mambient * lambient;

  // calculate diffuse color
  vec3 surf2light = normalize (lightDir);
  //vec3 norm = normalize (Normal);
  float dcont = max (0.0, dot (Normal, surf2light));
  vec3 diffuse = dcont * (mdiffuse * ldiffuse);

  // calculate specular color
  vec3 surf2view = normalize (-Vert_Pos);
  vec3 reflection = reflect (surf2light, Normal);

  //float scont = pow (max (0.0, dot(surf2view, reflection)), shininess);
  float scont = max (0.0, dot(surf2view, reflection)) / shininess;
  vec3 specular = scont * lspecular * mspecular;

  lightColor += vec4 ((ambient + diffuse + specular), 1.0);

  //Color + fog effect//////////////////////
  float visibility = 1250;
  float factor = Position.z / 1000; 
  float r = lightColor.r * (1.5 - color1.r + (color2.r) * Normal.r);
  float r_range = r - 0.5;
  float g = lightColor.g * (1.5 - color1.g + (color2.g) * Normal.g);
  float g_range = g - 0.5;
  float b = lightColor.b * (0.5 * color1.b + color2.b);
  float b_range = b - 0.5;

  float r_f = r;
  float g_f = g;
  float b_f = b;

  if(Position.z < -2 * visibility)
  {
    r_f = 0.5;
	g_f = 0.5;
	b_f = 0.5;
  }

  else if(Position.z < -visibility)
  {
    vec3 p = Position;
    r_f = ((-(p.z + visibility)) / visibility) * 0.5 + ((2 * visibility + p.z) / visibility) * r;
   
    g_f = ((-(p.z + visibility)) / visibility) * 0.5 + ((2 * visibility + p.z) / visibility) * g;
   
    b_f = ((-(p.z + visibility)) / visibility) * 0.5 + ((2 * visibility + p.z) / visibility) * b;
  }
  

  ////////////////////////////////////////

  outColor = vec4(r_f, g_f, b_f, 1.f);
}
