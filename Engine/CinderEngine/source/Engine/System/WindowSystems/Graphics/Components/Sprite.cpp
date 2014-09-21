#include "Sprite.h"
#include "Camera.h"

namespace Framework
{
  Sprite::Sprite (GameObject* go) : Component (go)
  {
    modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    drawable = false;
  }


  // NON-DEFAULT CONSTRUCTOR
  // MUST SPECIFY SHADER AND TEXTURE
  // MESH AND TRI DATA WILL BE PASSED TO CREATE CUSTOM MESH
  // ELSE THE DEFAULT QUAD WILL BE USED
  Sprite::Sprite (GameObject* go, Shader* _shader, Texture* _texture /*=NULL*/) : Component (go)
  {
    //Create_Mesh (_meshData, _triData);

    shader = _shader;
    texture = _texture;
    drawable = true;
  }

  void Sprite::Create (Shader* _shader, Texture* _texture /*= TEXTURE_NONE*/)
  {
    shader = _shader;
    texture = _texture;
    Specify_Attributes ();
    drawable = true;
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

    if (texture->Get_ID() != TEXTURE_NONE)
    {
      GLint texAttrib = glGetAttribLocation (shader->Get_ID(), "texcoord");
      glEnableVertexAttribArray (texAttrib);
      glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*) (10 * sizeof(GLfloat)));

      glUniform1i (glGetUniformLocation (shader->Get_ID(), "image"), 0);

      // If Texture Is To Be Used, Use Draw Texture Method To Draw Sprite
      DrawFunction = &Sprite::Draw_Texture;
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
    //////////////////////////////////////////////////////////////////////////
    // THIS BLOCK WILL GO INTO TRANSFORM COMPONENT
    //////////////////////////////////////////////////////////////////////////
 
    // MODEL TO WORLD
    //glm::mat4 modelMatrix = glm::translate (glm::vec3 (rand()% 5, 0.0f, -1.0f));
    GLuint uniModel = glGetUniformLocation (shader->Get_ID(), "modelMatrix");
    glUniformMatrix4fv (uniModel, 1, GL_FALSE, glm::value_ptr (modelMatrix));

    // WORLD TO VIEW
    GLuint uniView = glGetUniformLocation (shader->Get_ID(), "viewMatrix");
    glUniformMatrix4fv (uniView, 1, GL_FALSE, glm::value_ptr (Camera::GetWorldToViewMatrix()));

    // VIEW TO PROJECTION
    GLuint uniProjection = glGetUniformLocation (shader->Get_ID(), "projectionMatrix");
    glUniformMatrix4fv (uniProjection, 1, GL_FALSE, glm::value_ptr (Camera::GetViewToProjectionMatrix()));

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    (this->*DrawFunction)();
  }


  // Draw Sprite Using Texture
  void Sprite::Draw_Texture ()
  {
    //Use_Shader (shader->Get_ID());
    glBindTexture (GL_TEXTURE_2D, texture->Get_ID());
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture (GL_TEXTURE_2D, 0);
  }


  // Draw Sprite Without Using Texture
  void Sprite::Draw_No_Texture ()
  {
    Use_Shader (shader->Get_ID());
    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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