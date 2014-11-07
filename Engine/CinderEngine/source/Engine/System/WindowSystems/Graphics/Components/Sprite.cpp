#include "Sprite.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "ShapeGenerator.h"
#include "WindowSystem.h"

namespace Framework
{
  DefineComponentName (Sprite);

  ZilchDefineType(Sprite, CinderZilch)
  {
  }

  VAO* Sprite::vao;
  VBO* Sprite::vbo;
  EBO* Sprite::ebo;


  void Sprite::Initialize ()
  {
    IGraphicsObject::Register ();
    gameObject->Sprite = this;

    if (vao == nullptr || vbo == nullptr || ebo == nullptr)
    {
      ShapeData data = ShapeGenerator::Generate_Quad ();
      vao = new VAO ();
      vbo = new VBO (data.vbo_size (), data.vertices);
      ebo = new EBO (data.ebo_size (), data.indices);
      Specify_Attributes ();
      vao->unbindVAO ();
    }
  }


  void Sprite::Serialize(Serializer::DataNode* data)
{
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // shader   : Shader*       Resources::RS->Get_Shader (Serialized String Name);
    // texture  : Texture*      Resources::RS->Get_Texture (Serialized String Name);
    // atlas    : SpriteSheet*  Resources::RS->Get_SpriteSheet (Serialized String Name);
    //////////////////////////////////////////////////////////////////////////
	  Serializer::DataNode* value = data->FindElement(data, "SpriteSource");
	  std::string texname;
	  value->GetValue(&texname);
	  texture = Resources::RS->Get_Texture(texname);

	  value = data->FindElement(data, "Shader");
	  std::string shadername;
	  value->GetValue(&shadername);
	  shader = Resources::RS->Get_Shader(shadername);

	  animated = false;
  }


  void Sprite::Create_Sprite (Shader* _shader, Texture* _texture /*= TEXTURE_NONE*/)
  {
    shader = _shader;
    texture = _texture;
    animated = false;
    Specify_Attributes ();
  }


  void Sprite::Create_Sprite (Shader* _shader, SpriteSheet* _atlas)
  {
    shader = _shader;
    texture = _atlas;
    atlas = _atlas;
    animated = true;
    Specify_Attributes ();
  }


  // Destructor
  Sprite::~Sprite ()
  {
    gameObject->Sprite = nullptr;
    WindowSystem::graphicsObjects.remove (this);
    if (vao != nullptr)
    {
      delete vao;
      vao = nullptr;
    }
    if (vbo != nullptr)
    {
      delete vbo;
      vbo = nullptr;
    }
    if (ebo != nullptr)
    {
      delete ebo;
      ebo = nullptr;
    }
  }


  // Call To Change Shader Used By Sprite
  void Sprite::Change_Shader (Shader* changeShader)
  {
    shader = changeShader;
    Specify_Attributes ();
  }

  // Call To Change Texture Used By Sprite
  void Sprite::Change_Texture (Texture* _texture)
  {
    texture = _texture;
    Specify_Attributes ();
  }


  // Used To Communicate With Shader and Specify Attributes from Vertex Data
  void Sprite::Specify_Attributes ()
  {
    // Specify the layout of the vertex data
    posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof (GLfloat), 0);

    colorAttrib = shader->attribLocation ("color");
    shader->enableVertexAttribArray (colorAttrib);
    shader->vertexAttribPtr (colorAttrib, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    normalAttrib = shader->attribLocation ("normal");
    shader->enableVertexAttribArray (normalAttrib);
    shader->vertexAttribPtr (normalAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 7 * sizeof(GLfloat));

    if (animated || texture->Get_ID () != TEXTURE_NONE)
    {
      texAttrib = shader->attribLocation ("texcoord");
      shader->enableVertexAttribArray (texAttrib);
      shader->vertexAttribPtr (texAttrib, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 10 * sizeof(GLfloat));

      if (animated)
      {
        DrawFunction = &Sprite::Draw_Animated;
        uniTexOffset = shader->uniLocation ("texOffset");
        frameRatio.x = 1.0f / atlas->Get_Columns ();
        frameRatio.y = 1.0f / atlas->Get_Rows ();
        uniFrameRatio = shader->uniLocation ("frameRatio");
        shader->uni2fv ("frameRatio", glm::value_ptr (frameRatio));
      }
      else
      {
		  texture->Bind();
		  shader->uni1i("image", 0);
		  texture->Unbind();
        // If Texture Is To Be Used, Use Draw Texture Method To Draw Sprite
        DrawFunction = &Sprite::Draw_Texture;
      }
    }
    else
    {
      // If Texture Is Not Used, Use Draw No Texture Method To Draw Sprite
      DrawFunction = &Sprite::Draw_No_Texture;
    }

  }


  // This Is To Create A Custom Mesh
  // Not Complete
  void Sprite::Create_Mesh (GLfloat* vertices, GLuint* indices, GLuint arraySize)
  {
  }


  // Called By Renderer Component
  void Sprite::Draw ()
  {
    vao->bindVAO ();
    shader->Use ();
    shader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));

    (this->*DrawFunction)();
    shader->Disable ();
    vao->unbindVAO ();
  }


  // Draw Sprite Using Texture
  void Sprite::Draw_Texture ()
  {
    texture->Bind ();
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    texture->Unbind ();
  }


  // Draw Sprite Without Using Texture
  void Sprite::Draw_No_Texture ()
  {
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }


  void Sprite::Draw_Animated ()
  {
    //Specify_Attributes ();

    ++frameNumber;
    if (frameNumber % atlas->Get_Samples() == 0)
    {
      texOffset.x += frameRatio.x;
      if (texOffset.x == 1.0f)
      {
        texOffset.x = 0.0f;
        texOffset.y += frameRatio.y;
        if (texOffset.y == 1.0f)
        {
          texOffset.y = 0.0f;
        }
      }
    }
    glUniform2fv (uniFrameRatio, 1, glm::value_ptr (frameRatio));
    glUniform2fv (uniTexOffset, 1, glm::value_ptr (texOffset));

    atlas->Bind ();
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    atlas->Unbind ();
  }


  // Use Shader ID as the Shader
  void Sprite::Use_Shader (GLuint _shader)
  {
    glUseProgram (_shader);
  }

  void Sprite::Update_Shader ()
  {

  }

  // Gettors
  Shader* Sprite::Get_Shader ()
  {
    return shader;
  }

  Texture* Sprite::Get_Texture ()
  {
    return texture;
  }

}