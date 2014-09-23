#ifndef _GRAPHICS_COMMON_H
#define _GRAPHICS_COMMON_H

#include "glew.h"
#include "glfw3.h"
#include "CinderMath.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Resources.h"
#include "ShapeGenerator.h"
#include "Color.h"
#include <fstream>
#include <iostream>
#endif

#pragma region BACKUP
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Create and compile the vertex shader
//GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
//glShaderSource (vertexShader, 1, &vertexSource, NULL);
//glCompileShader (vertexShader);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Create and compile the fragment shader
//GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
//glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
//glCompileShader (fragmentShader);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Link the vertex and fragment shader into a shader program
//GLuint shaderProgram = glCreateProgram ();
//glAttachShader (shaderProgram, vertexShader);
//glAttachShader (shaderProgram, fragmentShader);
//glBindFragDataLocation (shaderProgram, 0, "outColor");
//glLinkProgram (shaderProgram);
//glUseProgram (shaderProgram);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//// Load texture
//GLuint tex;
//glGenTextures (1, &tex);

//int width, height;
//unsigned char* image = SOIL_load_image ("Default.jpg", &width, &height, 0, SOIL_LOAD_RGB);
//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//SOIL_free_image_data (image);

//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//////////////////////////////////////////////////////////////////////////
#pragma endregion