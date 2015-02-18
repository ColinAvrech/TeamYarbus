#version 430 core
//Generate a fullscreen quad
#define QUAD_SIZE 2.755868545
//split screen
//const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2((QUAD_SIZE-1) * 0.5, -1.0), vec2(-1.0, QUAD_SIZE), vec2((QUAD_SIZE-1)*0.5, QUAD_SIZE) };
//overlay
const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.8125, -1.0), vec2(-1.0, 1.8125), vec2(1.8125, 1.8125) };

//split screen
//const vec2 texCoords[4] = {vec2(0.0, 0.0), vec2(0.5, 0.0), vec2(0.0, 1.0), vec2(0.5, 1.0)};
//overlay
const vec2 texCoords[4] = {vec2(0.0, 0.0), vec2(1, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0)};

varying vec2 Texcoord;
void main()
{
    Texcoord = texCoords[gl_VertexID];
    gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}