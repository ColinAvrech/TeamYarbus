#version 430

in vec2 coord2d;
in vec4 graph_coord;
in float temperature;
uniform mat4 texture_transform;
uniform mat4 vertex_transform;
uniform sampler2D mytexture;

out vec4 Graph_Coord;
out float Temperature;

void main(void)
{
  vec4 gc = graph_coord;
	gc = texture_transform * vec4(coord2d, 0, 1);
	gc.z = (texture2D(mytexture, gc.xy / 2.0 + 0.5).r);
  Graph_Coord = gc;
  Temperature = temperature / 1000;
	gl_Position = vertex_transform * vec4(coord2d, gc.z, 1);
}
