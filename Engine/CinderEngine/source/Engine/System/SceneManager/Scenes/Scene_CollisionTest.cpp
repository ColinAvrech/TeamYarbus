/******************************************************************************/
/*!
\file   Scene_CollisionTest.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Scene_CollisionTest.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "DebugCircle.h"
#include "ColliderShape.h"
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"

namespace Framework
{
  using namespace Physics;

  DebugCircleRenderer debugRenderer;
  int circleDivisions = 40;
  float circleRadius = 0.3f;
  Circle circle (NULL), circle1 (NULL);
  Sprite sprite (NULL), sprite1 (NULL);
  VAO* vao;
  VBO* vbo;
  EBO* ebo;
  GLuint ARRAY_BUFFER;

  // Constructor
  Scene_CollisionTest::Scene_CollisionTest ()
  {
  }

  // Destructor
  Scene_CollisionTest::~Scene_CollisionTest ()
  {
    delete vao, vbo, ebo;
  }

  static void OnKeyPressed (GameObject* go, KeyEvent* _key)
  {
    //int key = _key->KeyValue;
    //int action = (int)_key->KeyDown;
    //if (key == GLFW_KEY_D && action == GLFW_PRESS && circleDivisions < 85)
    //  circleDivisions += 1;
    //if (key == GLFW_KEY_A && action == GLFW_PRESS &&  circleDivisions > 3)
    //  circleDivisions -= 1;
    //if (key == GLFW_KEY_W && action == GLFW_PRESS)
    //  circle.SetRadius (circle.GetRadius() + 0.025f);
    //if (key == GLFW_KEY_S && action == GLFW_PRESS && circleRadius >= 0.1f)
    //  circle.SetRadius (circle.GetRadius () - 0.025f);

    if (_key->KeyDown)
    switch (_key->KeyValue)
    {
    case GLFW_KEY_A:
      sprite.transform.Translate (-0.01f, 0, 0);
      break;
    case GLFW_KEY_D:
      sprite.transform.Translate (0.01f, 0, 0);
      break;
    case GLFW_KEY_S:
      sprite.transform.Translate (0, -0.01f, 0);
      break;
    case GLFW_KEY_W:
      sprite.transform.Translate (0, 0.01f, 0);
      break;
    default:
      break;
    }
  }


  void Scene_CollisionTest::Load_Scene (const char* filename)
  {
    EVENTSYSTEM->Connect (NULL, Events::KEY_ANY, BaseEvent::BaseCall (OnKeyPressed));
    debugRenderer = DebugCircleRenderer ();
    debugRenderer.Generate_Buffers ();
    debugRenderer.Generate_Shaders ();

    ShapeData data = ShapeGenerator::Generate_Quad ();

    vao = new VAO ();
    vbo = new VBO (data.vbo_size (), data.vertices);
    ebo = new EBO (data.ebo_size (), data.indices);
    sprite.Create_Sprite (Resources::RS->Get_Shader ("Default"), Resources::RS->Get_Texture ("Circle.png"));
    sprite.transform.Translate (-0.5f, 0.0f, -1.0f);
    sprite.transform.Scale (0.1f);

    sprite1.Create_Sprite (Resources::RS->Get_Shader ("Default"), Resources::RS->Get_Texture ("Circle.png"));
    sprite1.transform.Translate (0.5f, 0.0f, -1.0f);
    sprite1.transform.Scale (0.1f);
    data.Clean ();
  }


  void Scene_CollisionTest::Update (const double dt)
  {
  }


  void Scene_CollisionTest::Draw ()
  {
    glClearColor (1, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vao->bindVAO ();
    sprite.Draw ();
    sprite1.Draw ();
    vao->unbindVAO ();
    debugRenderer.Draw (&sprite.transform, &circle);
    debugRenderer.Draw (&sprite1.transform, &circle1);

    glDisable (GL_BLEND);
  }
  
  
  void Scene_CollisionTest::Change_Size (int x, int y)
  {
    glViewport (0, 0, x, y);
  }

}