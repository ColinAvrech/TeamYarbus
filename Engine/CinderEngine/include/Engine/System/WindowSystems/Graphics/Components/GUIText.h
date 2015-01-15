/******************************************************************************/
/*!
\file   Text.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TEXT_H
#define _TEXT_H

#include <string>
#include "glm.hpp"
#include "Component.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Framework
{
  class GUIText : public IGraphicsObject
  {
  public:
	ZilchDeclareDerivedType(GUIText, IGraphicsObject);
    GUIText();
    virtual ~GUIText();

	FT_Library*  library;
	
	FT_Face  face;

    virtual void Serialize (Serializer::DataNode* data);
    virtual void Initialize ();
    virtual void OnApplicationPause (PauseEvent* pause);

	void GUIText::Change_Color(Real4 newcolor);
    // X, Y BETWEEN -1 to +1
    virtual void Draw ();

    void ToggleVisibility();

	//Serialized values
    bool visible = true;
    vec2 position = vec2(0,0);
    String text = "Hello World";
	Real4 color = Real4(1,1,1,1);
	int size = 8;
	String font = String("Default.ttf");
	Real3 scale = Real3(1,1,1);

	const static string Name;

	virtual bool Draw_Last() { return false; }
	virtual bool InViewport() { return true; }

  
  private:
	static VAO* vao;

  };  
}

#endif