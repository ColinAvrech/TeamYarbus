/******************************************************************************/
/*!
\file   FractalGenerator.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "FractalGenerator.h"
#include "WindowSystem.h"
#include "Pipeline.h"
#include "ResourceManager.h"

namespace Framework
{

  int screenWidth = 1024;
  int screenHeight = int (1024 / (16 / 9.f));

  // Constructor
  FractalGenerator::FractalGenerator ()
  {
  }
  
  // Destructor
  FractalGenerator::~FractalGenerator ()
  {
  }

  void FractalGenerator::Generate_Tree ()
  {
    xPositions.clear ();
    yPositions.clear ();
    sizes.clear ();
    colors.clear ();
    angles.clear ();

    int randVarRed = rand () % 3;
    if (randVarRed == 0)
      treeRed = 0.35f;
    else if (randVarRed == 1)
      treeRed = 0.25f;
    else if (randVarRed == 2)
      treeRed = 0.15f;

    int randVarGreen = rand () % 2;
    if (randVarGreen == 0)
      treeGreen = 0.15f;
    else if (randVarGreen == 1)
      treeGreen = 0.30f;

    Generate_Fractal (0, 0, (rand () % screenHeight / 4) + 40.0f, 0.0f);

    //joints->clear ();
    //int r0 = rand () % xPositions.size ();
    //joints->push_back ({ xPositions.at (r0), yPositions.at (r0) });
    //int r1 = rand () % xPositions.size ();
    //joints->push_back ({ xPositions.at (r1), yPositions.at (r1) });
    //int r2 = rand () % xPositions.size ();
    //joints->push_back ({ xPositions.at (r2), yPositions.at (r2) });
    //int r3 = rand () % xPositions.size ();
    //joints->push_back ({ xPositions.at (r3), yPositions.at (r3) });
  }

  void FractalGenerator::Generate_Fractal (GLfloat xPos, GLfloat yPos, GLfloat size, GLfloat degrees)
  {
    if (size < 5.0f || yPos - size > 10.0f)
    {
      return;
    }
    static const int maxShrink = 900; // per thousand
    static const int minShrink = 1;
    static const int maxAngle = 45;
    static const int maxBranches = 8;
    static const int minBranches = 0;
    GLfloat newPosX = xPos + size * sin (degrees / 180.0f * M_PI);
    GLfloat newPosY = yPos - size * cos (degrees / 180.0f * M_PI);

    xPositions.push_back (xPos / screenWidth);
    yPositions.push_back (yPos / screenWidth);
    colors.push_back (size);
    sizes.push_back (size / screenWidth);
    angles.push_back (degrees);

    int branches = rand () % (maxBranches - minBranches) + 1 + minBranches;

      //joints->push_back ({ xPos / screenWidth, yPos / screenHeight });
    for (int i = 0; i < branches; ++i)
    {
      int x = rand () % 2 == 1 ? -1 : 1;
      Generate_Fractal (newPosX, newPosY, size * (rand () % maxShrink + minShrink) / 1000.0f, degrees + x * rand () % maxAngle);
    }
  }

  void FractalGenerator::Create_Mesh(int lines, std::vector <float>* mesh, std::vector <glm::vec2>* edges)
{
    mesh->clear ();
    edges->clear ();
    screenWidth = WINDOWSYSTEM->Get_Width ();
    screenHeight = WINDOWSYSTEM->Get_Height ();

    OPENGL->MatrixMode (MODEL);
    OPENGL->LoadIdentity ();
    OPENGL->Scalef (1.0f, -1.0f * ((float)screenWidth / screenHeight), 1.0f);

    if (lines > (int) xPositions.size ())
    {
      return;
    }

    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec4 color;

    std::cout << lines << "\n";

    float right = *std::max_element (xPositions.begin (), xPositions.end ());
    float left = *std::min_element (xPositions.begin (), xPositions.end ());
    float top = *std::max_element (yPositions.begin (), yPositions.end ());
    float bottom = *std::min_element (yPositions.begin (), yPositions.end ());
    float height = (top - bottom);
    float center = (left + right) / 2;
    //edges->push_back ({ center, bottom + height * 2.0f });
    ////edges->push_back ({ right, bottom + 0.5f });
    //edges->push_back ({ left, top + height });
    //edges->push_back ({ right, top + height });

    edges->push_back ({ center, height * 1.35f });

    if (lines >= 128)
    {
      //edges->push_back ({ right * 0.5f, height * 0.5f });
      //edges->push_back ({ left * 0.5f, height * 0.5f });
      if (lines >= 512)
      {
        edges->push_back ({ right, height });
        edges->push_back ({ left, height });
      }
      if (lines >= 2048)
      {
        edges->push_back ({ right * 0.75f, height * 1.25f });
        edges->push_back ({ left * 0.75f, height * 1.25f });
      }
      if (lines >= 4096)
      {
        edges->push_back ({ center, height * 1.5f });
        edges->push_back ({ right * 0.35f, height * 1.25f });
        edges->push_back ({ left * 0.35f, height * 1.25f });
        edges->push_back ({ center, height });
      }
    }

    for (int i = 0; i < lines; ++i)
    {
      OPENGL->PushMatrix ();
      OPENGL->Translatef (xPositions.at (i), yPositions.at (i), 0.0f);
      OPENGL->Rotatef (angles.at (i), 0.0f, 0.0f, 1.0f);
      p1 = glm::vec2 (OPENGL->GetModelMatrix () * glm::vec4 (0, 0, 0, 1));
      p2 = glm::vec2 (OPENGL->GetModelMatrix () * glm::vec4 (0, -sizes.at(i), 0, 1));
      OPENGL->PopMatrix ();

      color = glm::vec4 (0.5f / (colors.at (i) * treeRed), 0.5f / (colors.at (i) * treeGreen), 0.0f, 1.0f);
      //if (lines > 1 && i % (lines / 2) == 0)
      //{
      //  int i = rand () % 1;
      //  if (i == 0)
      //  {
      //    edges->push_back ({ p1.x, p1.y });
      //  }
      //  else
      //  {
      //    edges->push_back ({ p2.x, p2.y });
      //  }
      //}
      mesh->push_back (p1.x);
      mesh->push_back (p1.y);
      mesh->push_back (color.r);
      mesh->push_back (color.g);
      mesh->push_back (color.b);
      mesh->push_back (color.a);
      mesh->push_back (p2.x);
      mesh->push_back (p2.y);
      mesh->push_back (color.r);
      mesh->push_back (color.g);
      mesh->push_back (color.b);
      mesh->push_back (color.a);
    }

    OPENGL->MatrixMode (MODEL);
    OPENGL->LoadIdentity();
  }

}