#include "Sprite.h"
#include "Camera.h"

namespace Framework
{
  Sprite::Sprite ()
  {
    modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    animated = false;
  }


  // NON-DEFAULT CONSTRUCTOR
  // MUST SPECIFY SHADER AND TEXTURE
  // MESH AND TRI DATA WILL BE PASSED TO CREATE CUSTOM MESH
  // ELSE THE DEFAULT QUAD WILL BE USED
  Sprite::Sprite (Shader* _shader, Texture* _texture /*=NULL*/)
  {
    //Create_Mesh (_meshData, _triData);

    shader = _shader;
    texture = _texture;
    animated = false;
  }


  Sprite::Sprite (Shader* _shader, SpriteSheet* _atlas)
  {
    shader = _shader;
    texture = _atlas;
    atlas = _atlas;
    animated = false;
    Specify_Attributes ();
  }


  void Sprite::Create (Shader* _shader, Texture* _texture /*= TEXTURE_NONE*/)
  {
    shader = _shader;
    texture = _texture;
    animated = false;
    Specify_Attributes ();
  }


  void Sprite::Create (Shader* _shader, SpriteSheet* _atlas)
  {
    shader = _shader;
    texture = _atlas;
    atlas = _atlas;
    animated = true;
    Specify_Attributes ();
  }


  // Destructor
  // Destroy All Buffers Used By Sprite
  // This Might Be Used For Derived Classes Which Will Need Special Buffer Objects
  // This Destructor Does Nothing Right Now
  Sprite::~Sprite ()
  {
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
    GLint posAttrib = glGetAttribLocation (shader->Get_ID(), "position");
    glEnableVertexAttribArray (posAttrib);
    glVertexAttribPointer (posAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation (shader->Get_ID(), "color");
    glEnableVertexAttribArray (colAttrib);
    glVertexAttribPointer (colAttrib, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

    GLint normalAttrib = glGetAttribLocation (shader->Get_ID(), "normal");
    glEnableVertexAttribArray (normalAttrib);
    glVertexAttribPointer (normalAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (7 * sizeof(GLfloat)));

    if (animated || texture->Get_ID () != TEXTURE_NONE)
    {
      //modelMatrix = glm::scale (glm::vec3 (1, 1 / texture->Get_Aspect_Ratio (), 1.0f));

      GLint texAttrib = glGetAttribLocation (shader->Get_ID(), "texcoord");
      glEnableVertexAttribArray (texAttrib);
      glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (10 * sizeof(GLfloat)));

      glUniform1i (glGetUniformLocation (shader->Get_ID(), "image"), 0);

      if (animated)
      {
        DrawFunction = &Sprite::Draw_Animated;
        uniTexOffset = glGetUniformLocation (shader->Get_ID (), "texOffset");
        frameRatio.x = 1.0f / atlas->Get_Columns ();
        frameRatio.y = 1.0f / atlas->Get_Rows ();
        uniFrameRatio = glGetUniformLocation (shader->Get_ID (), "frameRatio");
        glUniform2fv (uniFrameRatio, 1, glm::value_ptr (frameRatio));
      }
      else
      {
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
    shader->Use ();

    //////////////////////////////////////////////////////////////////////////
    // THIS BLOCK WILL GO INTO TRANSFORM COMPONENT
    //////////////////////////////////////////////////////////////////////////
 
    shader->uniMat4 ("modelMatrix", glm::value_ptr (modelMatrix));
    shader->uniMat4 ("viewMatrix", (glm::value_ptr (Camera::GetWorldToViewMatrix ())));
    shader->uniMat4 ("projectionMatrix", (glm::value_ptr (Camera::GetViewToProjectionMatrix ())));

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    (this->*DrawFunction)();
    shader->Disable ();
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