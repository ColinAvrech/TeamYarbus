/******************************************************************************/
/*!
\file   Text.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Text.h"
#include "ZilchCompiledLib.h"
#include "Pipeline.h"
#include "glut.h"

namespace Framework
{

  // Constructor
  Text::Text ()
  {}
  
  // Destructor
  Text::~Text ()
  {
    OPENGL->textObjects.remove (this);
  }

  void Text::Draw (std::string text, float x, float y)
  {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    //glScalef (5, 5, 1);
    const int font = (int) GLUT_BITMAP_TIMES_ROMAN_24;
    std::string s = text;
    const char* c = s.c_str ();
    glRasterPos2f (x, y);
    for (unsigned i = 0; i < s.size (); ++i)
    {
      glutBitmapCharacter ((void*) font, *(c + i));
    }
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
  }

  void Text::Draw ()
  {
    Draw (text, position.x, position.y);
  }

  void Text::Initialize ()
  {
    OPENGL->textObjects.push_back (this);
  }

  void Text::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Position");
    value->GetValue (&position);

    value = data->FindElement (data, "Text");
    value->GetValue (&text);
  }

  void Text::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

}