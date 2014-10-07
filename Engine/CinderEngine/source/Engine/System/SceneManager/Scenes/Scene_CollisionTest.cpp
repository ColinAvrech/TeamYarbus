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
#include "ShapeCollider.h"
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"
#include "CollisionEvent.h"
#include "TimeQuery.h"

namespace Framework
{
  using namespace Physics;

  static GameObject* go1, *go2;
  static DebugCircleRenderer debugRenderer;
  static int circleDivisions = 40;
  static float circleRadius = 0.3f;
  static bool useDebug = false;
  static CircleCollider circle (NULL), circle1 (NULL);
  static Sprite sprite (NULL), sprite1 (NULL);
  static VAO* vao;
  static VBO* vbo;
  static EBO* ebo;

  // Constructor
  Scene_CollisionTest::Scene_CollisionTest ()
  {
  }

  // Destructor
  Scene_CollisionTest::~Scene_CollisionTest ()
  {
    delete vao, vbo, ebo, go1->Sprite, go1->CircleCollider, go1->Transform, go1, go2;
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
    {
      if (glfwGetKey (WINDOWSYSTEM->Get_Window(), GLFW_KEY_A))
        go1->Transform->Translate (-0.01f, 0, 0);
      else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_D))
        go1->Transform->Translate (0.01f, 0, 0);

      if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_S))
        go1->Transform->Translate (0, -0.01f, 0);
      else if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_W))
        go1->Transform->Translate (0, 0.01f, 0);
      if (glfwGetKey (WINDOWSYSTEM->Get_Window (), GLFW_KEY_C))
        useDebug = !useDebug;
    }
  }


  static void OnCollisionEnter (GameObject* go, CollisionEvent* collision)
  {
    collision->OtherObject->Transform->Translate (-collision->normal.x * 0.05f, -collision->normal.y * 0.05f, 0.0f);
  }


  void Scene_CollisionTest::Load_Scene (const char* filename)
  {
    EVENTSYSTEM->Connect (NULL, Events::KEY_ANY, BaseEvent::BaseCall (OnKeyPressed));
    EVENTSYSTEM->Connect (go2, Events::Types::COLLISION, BaseEvent::BaseCall (OnCollisionEnter));
    debugRenderer = DebugCircleRenderer ();
    debugRenderer.Generate_Buffers ();
    debugRenderer.Generate_Shaders ();

    go1 = new GameObject (0);
    go2 = new GameObject (1);

    go1->Transform = new Transform (go1);
    go1->Sprite = new Sprite (go1);
    go1->CircleCollider = new CircleCollider (go1);

    go2->Transform = new Transform (go2);
    go2->Sprite = new Sprite (go2);
    go2->CircleCollider = new CircleCollider (go2);

    ShapeData data = ShapeGenerator::Generate_Quad ();

    vao = new VAO ();
    vbo = new VBO (data.vbo_size (), data.vertices);
    ebo = new EBO (data.ebo_size (), data.indices);

    go1->Sprite->Create_Sprite (Resources::RS->Get_Shader ("Default"), Resources::RS->Get_Texture ("Smoke.png"));
    go1->Transform->Translate (-0.5f, 0.0f, -1.0f);
    go1->Transform->Scale (0.1f);

    go2->Sprite->Create_Sprite (Resources::RS->Get_Shader ("Default"), Resources::RS->Get_Texture ("Circle.png"));
    go2->Transform->Translate (0.5f, 0.0f, -1.0f);
    go2->Transform->Scale (0.1f);
    data.Clean ();

    std::cout << Console::red << "Press C for Debug Draw\n" << Console::gray;
  }


  void Scene_CollisionTest::Update (const double dt)
  {
    go1->Transform->UpdateMatrices ();
    go2->Transform->UpdateMatrices ();
    go1->CircleCollider->DetectCircle (go2->CircleCollider);
  }


  void Scene_CollisionTest::Draw ()
  {
    glClearColor (1, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vao->bindVAO ();
    go1->Sprite->Draw ();
    go2->Sprite->Draw ();
    vao->unbindVAO ();

    if (useDebug)
    {
      debugRenderer.Draw (go1->CircleCollider);
      debugRenderer.Draw (go2->CircleCollider);
    }

    glDisable (GL_BLEND);
  }
  
  
  void Scene_CollisionTest::Change_Size (int x, int y)
  {
    glViewport (0, 0, x, y);
  }

}