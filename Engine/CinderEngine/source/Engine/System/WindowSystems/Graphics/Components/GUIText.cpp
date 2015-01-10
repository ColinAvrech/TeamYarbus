/******************************************************************************/
/*!
\file   Text.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "GUIText.h"
#include "ZilchCompiledLib.h"
#include "Pipeline.h"
#include "glut.h"
#include "ZInterface.h"


namespace Framework
{
	DefineComponentName(GUIText);
	ZilchDefineType(GUIText, CinderZilch) //, builder, type
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindFieldGetSetAs(text, "Text");
		//ZilchBindFieldGetSetAs(position, "Position");
	}
  // Constructor
  GUIText::GUIText()
  {
	  //gameObject->GUIText = this;
	shader = Resources::RS->Get_Shader("Text");
	color = Real4( 1, 0, 1, 1 );
    visible = true;

	error = FT_Init_FreeType(&library);
	if (error)
	{
		cout << "ERROR: FREETYPE LIBRARY NOT INITIALIZED!" << endl;
	}
	//MUST HAVE FONTS BE LOADED IN FIRST, THEN LOADED FROM MEMEORY
	error = FT_New_Face(library, "../../Resources/Fonts/Arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		cout << "ERROR: THE FONT FILE COULD BE OPENED AND READ BUT ITS FORMAT IS UNSUPPORTED!" << endl;
	}
	else if (error)
	{
		cout << "ERROR: THE FONT FILE FAILED TO OPEN!" << endl;
	}
	//MUST BE FIXED FOR EACH INDIVIDUAL RESOLUTION
	error = FT_Set_Char_Size(face, 0, 16 * 64, 96, 96);
	FT_Set_Pixel_Sizes(face, 0, 48);
	//FT_Set_Pixel_Sizes(face, 0, 48);
  }
  
  // Destructor
  GUIText::~GUIText()
  {
    OPENGL->textObjects.remove (this);
	if (vao != nullptr)
	{
		delete vao;
		vao = nullptr;
	}
	//gameObject->GUIText = nullptr;
  }

  VAO* GUIText::vao = nullptr;

  void GUIText::Draw()
  {
		
    if (visible)
    {

		vao->bindVAO();
		shader->Use();
		shader->uni4fv("overrideColor", glm::value_ptr(ZInterface::RealToVec(&color)));
		shader->uniMat4("mvp", glm::value_ptr(gameObject->Transform->GetModelViewProjectionMatrix()));

		FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */

		const char* c = text.c_str();
		float xPos = position.x;
		float yPos = position.y;
		glRasterPos2f(xPos, yPos);
		//
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint tex;
		GLuint uniform_tex = 0;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//


		for (unsigned i = 0; i < text.size(); ++i)
		{
			FT_UInt glyph_index = FT_Get_Char_Index(face, int(*(c + i)));
			error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			


			Texture* texture = new Texture(&slot->bitmap);
			//gameObject->Sprite->Change_Texture(texture);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);



			float x2 = xPos + slot->bitmap_left * gameObject->Transform->GetScale().x * 0.01f;
			float y2 = -yPos - slot->bitmap_top * gameObject->Transform->GetScale().y * 0.01f;
			float w = slot->bitmap.width * gameObject->Transform->GetScale().x * 0.01f;
			float h = slot->bitmap.rows * gameObject->Transform->GetScale().y * 0.01f;
			
			GLfloat box[4][4] = {
					{ x2, -y2, 0, 0 },
					{ x2 + w, -y2, 1, 0 },
					{ x2, -y2 - h, 0, 1 },
					{ x2 + w, -y2 - h, 1, 1 },
			};
			

			//texture->Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			texture->Unbind();

			xPos += (slot->advance.x >> 6) * gameObject->Transform->GetScale().x * 0.01f;
			yPos += (slot->advance.y >> 6) * gameObject->Transform->GetScale().x * 0.01f;
			glRasterPos2f(xPos, yPos);
			delete texture;
			texture = nullptr;
		}
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      glMatrixMode (GL_MODELVIEW);
      glLoadIdentity ();

	  shader->Disable();
	  vao->unbindVAO();
    }
  }

  void GUIText::Initialize()
  {
    OPENGL->textObjects.push_back (this);
	vao = new VAO();
	GLuint vbo;
	GLuint attribute_coord = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute_coord);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	vao->unbindVAO();
  }

  void GUIText::Serialize(Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "Position");
    value->GetValue (&position);

    value = data->FindElement (data, "Text");
    text = String(value->value_.String_->c_str());
  }

  void GUIText::Change_Color(Real4 newcolor)
  {
	  color = newcolor;

	  shader->Use();
	  shader->uni4fv("overrideColor", glm::value_ptr(ZInterface::RealToVec(&color)));
	  shader->Disable();
  }

  void GUIText::OnApplicationPause(PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void GUIText::ToggleVisibility()
  {
    visible = !visible;
  }
}