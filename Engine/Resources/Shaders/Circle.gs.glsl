#version 430

layout (points) in;
layout (triangle_strip, max_vertices=140) out;

in vec3 Color[];

out vec3 fragColor;

uniform float radius;
uniform int divisions;
uniform mat4 modelViewProjectionmatrix;

void main ()
{
	// Generate a circle based on points center position
	for (int i=0; i < divisions; i++)
  {
		float degInRad = i*3.14159f/180.0f * (360.0f/divisions);
		float degInRadB = (i+1)*3.14159f/180.0f * (360.0f/divisions);

		// For each subdivision we create a single triangle
		gl_Position = modelViewProjectionmatrix * gl_in[0].gl_Position;
		fragColor = Color[0];
		EmitVertex();

		gl_Position = modelViewProjectionmatrix * (gl_in[0].gl_Position + vec4(radius * cos(degInRad), radius * sin(degInRad), 0.0f, 0.0f));
		fragColor = Color[0];
		EmitVertex();

		gl_Position = modelViewProjectionmatrix * (gl_in[0].gl_Position + vec4(radius * cos(degInRadB), radius * sin(degInRadB), 0.0f, 0.0f));
		fragColor = Color[0];
		EmitVertex();

		EndPrimitive();
	}
}