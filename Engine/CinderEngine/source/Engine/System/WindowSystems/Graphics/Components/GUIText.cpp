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
#include "Font.h"
#include "ResourceManager.h"


namespace Framework
{
	DefineComponentName(GUIText);
	ZilchDefineType(GUIText, CinderZilch) //, builder, type
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindFieldGetSetAs(text, "Text");
		ZilchBindFieldGetSetAs(color, "Color");
		ZilchBindFieldGetSetAs(visible, "Visible");
		ZilchBindFieldGetSetAs(scale, "Scale");
		ZilchBindFieldGetSetAs(size, "Size");
		//ZilchBindFieldGetSetAs(position, "Position");
	}
  // Constructor
  GUIText::GUIText()
  {
	  
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
      Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
		vao->bindVAO();
		shader->Use();
		shader->uni4fv("overrideColor", glm::value_ptr(ZInterface::RealToVec(&color)));
		shader->uniMat4("mvp", glm::value_ptr(tform->GetModelViewProjectionMatrix()));

		FT_GlyphSlot  slot = face->glyph;  /* a small shortcut */

		const char* c = text.c_str();
		float xPos = position.x;
		float yPos = position.y;
		float scaleX = 0.01f * scale.x;
		float scaleY = 0.01f * scale.y;
		if (tform)
		{
			vec3* pos = &tform->GetPosition();
			xPos += pos->x;
			yPos += pos->y;

			vec3* scale = &tform->GetScale();
			scaleX *= scale->x;
			scaleY *= scale->y;
		}
		glRasterPos2f(xPos, yPos);

		GLuint tex;
		GLuint uniform_tex = 0;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);


		for (unsigned i = 0; i < text.size(); ++i)
		{
			FT_UInt glyph_index = FT_Get_Char_Index(face, int(*(c + i)));
			FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			


			Texture* texture = new Texture(&slot->bitmap);
			//gameObject->Sprite->Change_Texture(texture);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);

			float x2 = xPos + slot->bitmap_left * scaleX;
			float y2 = -yPos - slot->bitmap_top * scaleY;
			float w = slot->bitmap.width * scaleX;
			float h = slot->bitmap.rows * scaleY;
			
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

			xPos += (slot->advance.x >> 6) * scaleX;
			yPos += (slot->advance.y >> 6) * scaleY;
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
	  //gameObject->GUIText = this;
	  shader = Resources::RS->Get_Shader("Text");
	  //color = Real4( 1, 0, 1, 1 );
	  visible = true;
	  library = &Resources::RS->FontLibrary;

	  //error = FT_New_Face(library, "../../Resources/Fonts/Arial.ttf", 0, &face);
	  Font* fontFile = Resources::RS->Get_Font(font);
	  face = fontFile->face;

	  
	  FT_Set_Char_Size(face, 0, 16 * 64, WINDOWSYSTEM->dpi, WINDOWSYSTEM->dpi);
	  FT_Set_Pixel_Sizes(face, 0, size);

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
	if (value)
	{
		value->GetValue(&position);
	}

	value = data->FindElement(data, "Visible");
	if (value)
	{
		value->GetValue(&visible);
	}

    value = data->FindElement (data, "Text");
	if (value)
	{
		text = String(value->value_.String_->c_str());
	}

	value = data->FindElement(data, "Color");
	if (value)
	{
		vector<float>* val = value->value_.VecN_;
		color = Real4(val->at(0), val->at(1), val->at(2), val->at(3));
	}

	value = data->FindElement(data, "Scale");
	if (value)
	{
		vector<float>* val = value->value_.VecN_;
		scale = Real3(val->at(0), val->at(1), val->at(2));
	}

	value = data->FindElement(data, "Size");
	if (value)
	{
		value->GetValue(&size);
	}

	value = data->FindElement(data, "Font");
	if (value)
	{
		font = String(value->value_.String_->c_str());
	}
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