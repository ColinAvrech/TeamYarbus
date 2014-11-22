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

namespace Framework
{

  int screenWidth = 1024;
  int screenHeight = int (1024 / (16 / 9.f));

  // Constructor
  FractalGenerator::FractalGenerator ()
  {}
  
  // Destructor
  FractalGenerator::~FractalGenerator ()
  {}

  void FractalGenerator::Generate_Tree ()
  {
    xPositions.clear ();
    yPositions.clear ();
    sizes.clear ();
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

    Generate_Fractal (screenWidth / 2.0f, screenHeight - 20.0f, (rand () % screenHeight / 4) + 40.0f, 0.0f);
  }

  void FractalGenerator::Generate_Fractal (GLfloat xPos, GLfloat yPos, GLfloat size, GLfloat degrees)
  {
    if (size < 5.0f || yPos - size < 10.0f)
      return;

    static const int maxShrink = 900; // per thousand
    static const int minShrink = 1;
    static const int maxAngle = 45;
    static const int maxBranches = 8;
    static const int minBranches = 0;
    GLfloat newPosX = xPos + size * sin (degrees / 180.0f * M_PI);
    GLfloat newPosY = yPos - size * cos (degrees / 180.0f * M_PI);

    xPositions.push_back (xPos);
    yPositions.push_back (yPos);
    sizes.push_back (size);
    angles.push_back (degrees);

    int branches = rand () % (maxBranches - minBranches) + 1 + minBranches;
    for (int i = 0; i < branches; ++i)
    {
      int x = rand () % 2 == 1 ? -1 : 1;
      Generate_Fractal (newPosX, newPosY, size * (rand () % maxShrink + minShrink) / 1000.0f, degrees + x * rand () % maxAngle);
    }
  }

  void FractalGenerator::Draw_Tree (int lines)
  {
    screenWidth = WINDOWSYSTEM->Get_Width ();
    screenHeight = WINDOWSYSTEM->Get_Height ();
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0.0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(), 0.0, -1.0, 1.0);
    glMatrixMode (GL_MODELVIEW);

    if (lines > (int)xPositions.size ())
      return;

    for (int i = 0; i < lines; ++i)
    {
      glColor3f (1.0f / (sizes.at (i) * treeRed), 1.0f / (sizes.at (i) * treeGreen), 0.0f);
      glLineWidth (sizes.at (i) / 40.0f + 1.0f);
      glPushMatrix ();
      glTranslatef (xPositions.at (i), yPositions.at (i), 0.0f);
      glRotatef (angles.at (i), 0.0f, 0.0f, 1.0f);
      glBegin (GL_LINES);
      glVertex2f (0.0f, 0.0f);
      glVertex2f (0.0f, -sizes.at (i));
      glEnd ();
      glPopMatrix ();
    }
  }

}