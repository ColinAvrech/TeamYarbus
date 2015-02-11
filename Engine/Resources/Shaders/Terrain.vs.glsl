#version 430

in vec3 position;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 Position;
out vec3 original_pos;
out vec3 Normal;

void main ()
{
  mat4 nmvp = projection * view * model;
  //if(nmvp == mvp)
	gl_Position = mvp * vec4 (position, 1.0);
  //else
  //  gl_Position = vec4(position, 1.0);
  Position = vec3 (position.xy, 1.0);
  original_pos = position;
  Normal = vec3 (0,0,1);
}