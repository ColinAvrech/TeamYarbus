/******************************************************************************/
/*!
\file   CinderEngine_UI.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "CinderEngine_UI.h"
#include "WindowSystem.h"
#include "ResourceManager.h"
#include "EventSystem.h"
#include "GameEvent.h"
#include "UpdateEvent.h"
#include "Pipeline.h"
#include "Core.h"

namespace Framework
{
  DefineComponentName (UIBox);

  VAO* UIBox::vao;
  VBO* UIBox::vbo;
  EBO* UIBox::ebo;
  UIEvent* UIBox::uiEvent;

  bool Rect::Intersects (glm::vec2 normPos)
  {
    if (normPos.x > S_Min.x && normPos.x < S_Max.x &&
      normPos.y > S_Min.y && normPos.y < S_Max.y)
    {
      return true;
    }
    return false;
  }

  UIComponent::UIComponent ()
  {
    Register ();
  }

  UIComponent::~UIComponent ()
  {
    Deregister ();
  }

  void UIComponent::Register ()
  {
    OPENGL->uiObjects.push_back (this);
  }

  void UIComponent::Deregister ()
  {
    OPENGL->uiObjects.remove (this);
  }

  UIBox::~UIBox ()
  {
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

  void UIBox::Serialize (Serializer::DataNode* data)
  {
    Component::Get_Enabled(data);
    Serializer::DataNode* value = data->FindElement (data, "SpriteSource");
    std::string texName;
    value->GetValue (&texName);
    shader = Resources::RS->Get_Shader ("Default");
    texture = Resources::RS->Get_Texture (texName);
    value = data->FindElement (data, "NormalColor");
    value->GetValue (&normalColor);
    value = data->FindElement (data, "HoverColor");
    value->GetValue (&hoverColor);
    value = data->FindElement (data, "DownColor");
    value->GetValue (&downColor);
    value = data->FindElement (data, "Dimension");
    value->GetValue (&box.Dimension);
    value = data->FindElement (data, "Message");
    value->GetValue (&Message);
  }


  void UIBox::Initialize ()
  {
    if (vao == nullptr || vbo == nullptr || ebo == nullptr)
    {
      ShapeData data = ShapeGenerator::Generate_Quad ();
      vao = new VAO ();
      vbo = new VBO (data.vbo_size (), data.vertices);
      ebo = new EBO (data.ebo_size (), data.indices);
      vao->unbindVAO ();
      data.Clean ();
    }
    color = normalColor;
    vao->bindVAO ();
    Specify_Attributes ();
    vao->unbindVAO ();

    box.Min.x = gameObject->Transform->GetPosition ().x - box.Dimension.x * gameObject->Transform->GetScale ().x * 0.5f;
    box.Min.y = gameObject->Transform->GetPosition ().y - box.Dimension.y * gameObject->Transform->GetScale ().y * 0.5f;
    box.Max.x = gameObject->Transform->GetPosition ().x + box.Dimension.x * gameObject->Transform->GetScale ().x * 0.5f;
    box.Max.y = gameObject->Transform->GetPosition ().y + box.Dimension.y * gameObject->Transform->GetScale ().y * 0.5f;
    EVENTSYSTEM->mConnect<UpdateEvent, UIBox> (Events::UPDATEEVENT, this, &UIBox::UIUpdate);

    if (uiEvent == nullptr)
    {
      uiEvent = new UIEvent ();
      uiEvent->Message = "";
    }
  }

  void UIBox::UIUpdate (UpdateEvent* update)
  {
    if (enabled)
    {      
      glm::vec2 normPos = WINDOWSYSTEM->Get_Normalized_Mouse_Position ();
      box.S_Min = gameObject->Transform->GetNDCPosition (box.Min);
      box.S_Max = gameObject->Transform->GetNDCPosition (box.Max);
      //std::cout << "MIN " << box.S_Min.x << ", " << box.S_Min.y << "\n";
      //std::cout << "MAX " << box.S_Max.x << ", " << box.S_Max.y << "\n";

      if (box.Intersects (normPos))
      {
        color = hoverColor;
        if (glfwGetMouseButton (WINDOWSYSTEM->Get_Window (), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
          buttonState = GLFW_PRESS;
          color = downColor;
        }
        else if (glfwGetMouseButton (WINDOWSYSTEM->Get_Window (), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE
          && buttonState == GLFW_PRESS)
        {
          buttonState = GLFW_RELEASE;
          uiEvent->Message = Zilch::String (Message.c_str ());
          EVENTSYSTEM->TriggerEvent (Events::UI, *uiEvent);
        }
      }
      else
      {
        buttonState = GLFW_RELEASE;
        color = normalColor;
      }
    }
  }

  void UIBox::UIDraw ()
  {
    if (enabled)
    {
      vao->bindVAO ();
      shader->Use ();
      texture->Bind ();
      shader->uni4fv ("overrideColor", glm::value_ptr (color));
      shader->uniMat4 ("mvp",
        glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
      glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      texture->Unbind ();
      shader->Disable ();
      vao->unbindVAO ();

      //glMatrixMode (GL_PROJECTION);
      //glLoadIdentity ();
      //gluPerspective (Camera::main->GetFOV (), (float) WINDOWSYSTEM->Get_Width () / WINDOWSYSTEM->Get_Height (), 0, 100.0f);
      //glMatrixMode (GL_MODELVIEW);
      //glLoadIdentity ();

      //glm::vec3 eye = glm::vec3 (0, 0, 1) * Camera::main->GetSize () + glm::vec3 (Camera::main->gameObject->Transform->GetPosition ().x, Camera::main->gameObject->Transform->GetPosition ().y, 0);
      //glm::vec3 center = Camera::main->gameObject->Transform->GetPosition ();
      //glm::vec3 up = glm::vec3 (0, 1, 0);

      //gluLookAt (eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
      //glColor3f (1, 0, 0);
      //glBegin (GL_LINES);
      //{
      //  glVertex2f (box.Max.x, box.Max.y);
      //  glVertex2f (box.Min.x, box.Max.y);

      //  glVertex2f (box.Min.x, box.Max.y);
      //  glVertex2f (box.Min.x, box.Min.y);

      //  glVertex2f (box.Min.x, box.Min.y);
      //  glVertex2f (box.Max.x, box.Min.y);

      //  glVertex2f (box.Max.x, box.Min.y);
      //  glVertex2f (box.Max.x, box.Max.y);
      //}
      //glEnd ();

      //glMatrixMode (GL_PROJECTION);
      //glLoadIdentity ();
      ////gluOrtho2D(0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(), 0);
      //gluOrtho2D (-1, 1, -1, 1);
      //glMatrixMode (GL_MODELVIEW);
      //glLoadIdentity ();
      //glColor3f (1, 0, 0);
      //glBegin (GL_LINES);
      //{
      //  glVertex2f (box.S_Max.x, box.S_Max.y);
      //  glVertex2f (box.S_Min.x, box.S_Max.y);

      //  glVertex2f (box.S_Min.x, box.S_Max.y);
      //  glVertex2f (box.S_Min.x, box.S_Min.y);

      //  glVertex2f (box.S_Min.x, box.S_Min.y);
      //  glVertex2f (box.S_Max.x, box.S_Min.y);

      //  glVertex2f (box.S_Max.x, box.S_Min.y);
      //  glVertex2f (box.S_Max.x, box.S_Max.y);
      //}
      //glEnd ();
    }
  }

  void UIBox::Specify_Attributes ()
  {
    shader->Use ();

    // Specify the layout of the vertex data
    GLint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);

    GLint colorAttrib = shader->attribLocation ("color");
    shader->enableVertexAttribArray (colorAttrib);
    shader->vertexAttribPtr (colorAttrib, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    GLint normalAttrib = shader->attribLocation ("normal");
    shader->enableVertexAttribArray (normalAttrib);
    shader->vertexAttribPtr (normalAttrib, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 7 * sizeof(GLfloat));

    shader->uni4fv ("overrideColor", glm::value_ptr (color));

    GLint texAttrib = shader->attribLocation ("texcoord");
    shader->enableVertexAttribArray (texAttrib);
    shader->vertexAttribPtr (texAttrib, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 10 * sizeof(GLfloat));

    texture->Bind ();
    shader->uni1i ("image", 0);
    texture->Unbind ();

    shader->Disable ();
  }

}