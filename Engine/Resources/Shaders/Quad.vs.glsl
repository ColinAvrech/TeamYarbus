#version 330 core
//Generate a fullscreen quad
const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
const vec2 texCoords[4] = {vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0)};
out vec2 texCoord;
void main()
{
    texCoord = texCoords[gl_VertexID];
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}