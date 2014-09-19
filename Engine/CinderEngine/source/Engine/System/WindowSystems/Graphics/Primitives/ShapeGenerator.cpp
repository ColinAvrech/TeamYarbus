/******************************************************************************/
/*!
\file   ShapeGenerator.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "ShapeGenerator.h"
#define ARRAY_SIZE(a) sizeof (a) / sizeof (*a)

namespace Framework
{

  //// Constructor
  //ShapeGenerator::ShapeGenerator ()
  //{}
  //
  //// Destructor
  //ShapeGenerator::~ShapeGenerator ()
  //{}

  ShapeData ShapeGenerator::Generate_Quad ()
  {
    ShapeData data;

    VertexData vd [] =
    {
      // 1
      glm::vec3 (-0.5f, 0.5f, 0.0f),
      glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec3 (0.0f, 0.0f, 1.0f),
      glm::vec2 (0.0f, 0.0f),
      // 2
      glm::vec3 (0.5f, 0.5f, 0.0f),
      glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec3 (0.0f, 0.0f, 1.0f),
      glm::vec2 (1.0f, 0.0f),
      // 3
      glm::vec3 (0.5f, -0.5f, 0.0f),
      glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec3 (0.0f, 0.0f, 1.0f),
      glm::vec2 (1.0f, 1.0f),
      // 4
      glm::vec3 (-0.5f, -0.5f, 0.0f),
      glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec3 (0.0f, 0.0f, 1.0f),
      glm::vec2 (0.0f, 1.0f)
    };

    GLuint ind [] =
    {
      0, 1, 2,
      2, 3, 0
    };

    data.numVertices = ARRAY_SIZE (vd);
    data.vertices = new VertexData [data.numVertices];
    memcpy (data.vertices, vd, sizeof (vd));

    data.numIndices = ARRAY_SIZE (ind);
    data.indices = new GLuint [data.numIndices];
    memcpy (data.indices, ind, sizeof (ind));

    return data;
  }

  ShapeData ShapeGenerator::Generate_Cube ()
  {
    ShapeData data;
    VertexData stackVerts [] =
    {
      glm::vec3 (-1.0f, 1.0f, 1.0f        ),  // 0
      glm::vec4 (1.0f, 0.0f, 0.0f, 1.0f   ),	// Color
      glm::vec3 (0.0f, 1.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, 1.0f, 1.0f         ),  // 1
      glm::vec4 (0.0f, 1.0f, 0.0f, 1.0f   ),	// Color
      glm::vec3 (0.0f, 1.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, 1.0f, -1.0f        ),  // 2
      glm::vec4 (0.0f, 0.0f, 1.0f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 1.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, 1.0f, -1.0f       ),  // 3
      glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 1.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),

      glm::vec3 (-1.0f, 1.0f, -1.0f       ),  // 4
      glm::vec4 (1.0f, 0.0f, 1.0f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, -1.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, 1.0f, -1.0f        ),  // 5
      glm::vec4 (0.0f, 0.5f, 0.2f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, -1.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, -1.0f, -1.0f       ),  // 6
      glm::vec4 (0.8f, 0.6f, 0.4f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, -1.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, -1.0f      ),  // 7
      glm::vec4 (0.3f, 1.0f, 0.5f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, -1.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),

      glm::vec3 (1.0f, 1.0f, -1.0f        ),  // 8
      glm::vec4 (0.2f, 0.5f, 0.2f, 1.0f   ),  // Color
      glm::vec3 (1.0f, 0.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, 1.0f, 1.0f         ),  // 9
      glm::vec4 (0.9f, 0.3f, 0.7, 1.0f    ),  // Color
      glm::vec3 (1.0f, 0.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, -1.0f, 1.0f        ),  // 10
      glm::vec4 (0.3f, 0.7f, 0.5f, 1.0f   ),  // Color
      glm::vec3 (1.0f, 0.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, -1.0f, -1.0f       ),  // 11
      glm::vec4 (0.5f, 0.7f, 0.5f, 1.0f   ),  // Color
      glm::vec3 (1.0f, 0.0f, 0.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),

      glm::vec3 (-1.0f, 1.0f, 1.0f        ),  // 12
      glm::vec4 (0.7f, 0.8f, 0.2f, 1.0f   ),  // Color
      glm::vec3 (-1.0f, 0.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, 1.0f, -1.0f       ),  // 13
      glm::vec4 (0.5f, 0.7f, 0.3f, 1.0f   ),  // Color
      glm::vec3 (-1.0f, 0.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, -1.0f      ),  // 14
      glm::vec4 (0.4f, 0.7f, 0.7f, 1.0f   ),  // Color
      glm::vec3 (-1.0f, 0.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, 1.0f       ),  // 15
      glm::vec4 (0.2f, 0.5f, 1.0f, 1.0f   ),  // Color
      glm::vec3 (-1.0f, 0.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),

      glm::vec3 (1.0f, 1.0f, 1.0f         ),  // 16
      glm::vec4 (0.6f, 1.0f, 0.7f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, 1.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, 1.0f, 1.0f        ),  // 17
      glm::vec4 (0.6f, 0.4f, 0.8f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, 1.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, 1.0f       ),  // 18
      glm::vec4 (0.2f, 0.8f, 0.7f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, 1.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, -1.0f, 1.0f        ),  // 19
      glm::vec4 (0.2f, 0.7f, 1.0f, 1.0f   ),  // Color
      glm::vec3 (0.0f, 0.0f, 1.0f         ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),

      glm::vec3 (1.0f, -1.0f, -1.0f       ),  // 20
      glm::vec4 (0.8f, 0.3f, 0.7f, 1.0f   ),  // Color
      glm::vec3 (0.0f, -1.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, -1.0f      ),  // 21
      glm::vec4 (0.8f, 0.9f, 0.5f, 1.0f   ),  // Color
      glm::vec3 (0.0f, -1.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (-1.0f, -1.0f, 1.0f       ),  // 22
      glm::vec4 (0.5f, 0.8f, 0.5f, 1.0f   ),  // Color
      glm::vec3 (0.0f, -1.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               ),
      glm::vec3 (1.0f, -1.0f, 1.0f        ),  // 23
      glm::vec4 (0.9f, 1.0f, 0.2f, 1.0f   ),  // Color
      glm::vec3 (0.0f, -1.0f, 0.0f        ),  // Normal
      glm::vec2 (0.0f, 0.0f               )
    };

    data.numVertices = ARRAY_SIZE (stackVerts);
    data.vertices = new VertexData [data.numVertices];
    memcpy (data.vertices, stackVerts, sizeof(stackVerts));

    GLuint stackIndices [] = {
      0, 1, 2, 0, 2, 3, // Top
      4, 5, 6, 4, 6, 7, // Front
      8, 9, 10, 8, 10, 11, // Right 
      12, 13, 14, 12, 14, 15, // Left
      16, 17, 18, 16, 18, 19, // Back
      20, 22, 21, 20, 23, 22, // Bottom
    };
    data.numIndices = ARRAY_SIZE (stackIndices);
    data.indices = new GLuint [data.numIndices];
    memcpy (data.indices, stackIndices, sizeof(stackIndices));

    return data;
  }

}