/******************************************************************************/
/*!
\file   Scene0.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Scene_SplashScreens.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "SceneManager.h"

namespace Framework
{
  //namespace Splash_Screen
  //{
  Camera camera (NULL, true);
  Transform light (NULL);
  Sprite sprite (NULL);
  Sprite sprite1 (NULL);
  float camScrollSpeed = 0.05f;
  float shininess = 100.0f;
  bool isPressed = false;
  VAO* vao;
  VBO* vbo;
  EBO* ebo;
  glm::vec2 texoffset (0.0f, 0.0f);
  const int samples = 4;
  bool fadeOut;
  static int frames;


  // Constructor
  Scene_SplashScreens::Scene_SplashScreens ()
  {
    //Load_Scene (filename);
  }

  // Destructor
  Scene_SplashScreens::~Scene_SplashScreens ()
  {
    delete vao;
    delete vbo;
    delete ebo;
  }


  void Scene_SplashScreens::Key_Pressed (GLFWwindow* window, int key, int scanCode, int state, int mods)
  {
    //if (state == GLFW_REPEAT)
    switch (key)
    {
    case GLFW_KEY_W:
      light.Translate (0, 0.01f, 0);
      //SoundName.test2->SetPause(true);
      break;
    case GLFW_KEY_S:
      light.Translate (0, -0.01f, 0);
      //SoundName.test2->SetPause(false);
      break;
    case GLFW_KEY_D:
      light.Translate (0.01f, 0, 0);
      break;
    case GLFW_KEY_A:
      light.Translate (-0.01f, 0, 0);
      break;
    case GLFW_KEY_Q:
      shininess += 0.5f;
      break;
    case GLFW_KEY_E:
      shininess -= 0.5f;
      break;
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose (window, GL_TRUE);
      break;
    case GLFW_KEY_LEFT:
      //////////////////////////////////////////////////////////////////////////
      // Only For Testing
      sprite.modelMatrix = glm::translate (sprite.modelMatrix, glm::vec3 (-0.1f, 0.0f, 0.0f));
      //////////////////////////////////////////////////////////////////////////
      //camera.UpdatePosition (glm::vec3 (-camScrollSpeed, 0.0f, 0.0f));
      break;
    case GLFW_KEY_RIGHT:
      //////////////////////////////////////////////////////////////////////////
      // Only Fore Testing
      sprite.modelMatrix = glm::translate (sprite.modelMatrix, glm::vec3 (0.1f, 0.0f, 0.0f));
      //////////////////////////////////////////////////////////////////////////
      //camera.UpdatePosition (glm::vec3 (camScrollSpeed, 0.0f, 0.0f));
      break;
    case GLFW_KEY_UP:
      camera.UpdatePosition (glm::vec3 (0.0f, camScrollSpeed, 0.0f));
      break;
    case GLFW_KEY_DOWN:
      camera.UpdatePosition (glm::vec3 (0.0f, -camScrollSpeed, 0.0f));
      break;
    case GLFW_KEY_Z:
      camera.Zoom (0.01f);
      break;
    case GLFW_KEY_X:
      camera.Zoom (-0.01f);
      break;
    default:
      break;
    }
  }


  void Scene_SplashScreens::Mouse_Button (GLFWwindow* window, int button, int action, int mods)
  {
    if (action == GLFW_PRESS)
      switch (button)
    {
      case GLFW_MOUSE_BUTTON_1:
        isPressed = true;
        std::cout << "LMB Pressed\n";
        break;
      default:
        break;
    }
    else
    {
      isPressed = false;
    }
  }


  void Scene_SplashScreens::Mouse_Moved (float xPos, float yPos)
  {
    if (isPressed)
    {
      camera.MouseUpdate (glm::vec2 (float (xPos), float (yPos)));
    }
  }


  void Scene_SplashScreens::Load_Scene (const char* filename)
  {
    fadeOut = false;
    light.Translate (0, 0.36f, 0);
    // Generate a Quad for Us
    ShapeData quad = ShapeGenerator::Generate_Quad ();

    vao = new VAO ();
    vbo = new VBO (quad.vbo_size (), quad.vertices);
    ebo = new EBO (quad.ebo_size (), quad.indices);

    // Free Allocated Memory
    quad.Clean ();

    // Create Sprite
    sprite.Create
      (
      Resources::RS->Get_Shader ("LightingAnimation"),
      Resources::RS->Get_SpriteSheet ("Logo.png")
      );
    sprite1.Create
      (
      Resources::RS->Get_Shader ("Lighting"),
      Resources::RS->Get_Texture ("TeamName.png")
      );

    sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    sprite.modelMatrix = glm::scale (sprite.modelMatrix, glm::vec3 (0.5f, 0.5f, 1.0f));

   /* sprite1.modelMatrix = glm::translate (glm::vec3 (0, 0.5f, -1));
    sprite1.modelMatrix = glm::scale (sprite1.modelMatrix, glm::vec3 (0.5f, 0.5f / sprite1.Get_Texture ()->Get_Aspect_Ratio (), 1.0f));*/

    //SoundName.test1 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music2.mp3");
    //SoundName.test1->Play();
    //
    //SoundName.test2 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music.mp3");
    //SoundName.test2->Play();
  }


  void Scene_SplashScreens::Update (const double dt)
  {
    light.UpdateMatrices (0);

    std::cout << shininess << std::endl;

    if (shininess >= 4.0f && !fadeOut)
      shininess -= 0.5f;
    else
    {
      ++frames;
      fadeOut = true;
      if (frames > 60)
      shininess += 0.5f;
      if (shininess >= 100.0f)
      {
        SCENEMANAGER->Load_Next_Scene ();
      }
    }

    sprite.Get_Shader ()->Use ();
    // Update Uniform Variables
    sprite.Get_Shader ()->uni3fv ("lightPos", glm::value_ptr (light.GetPosition ()));
    sprite.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
    sprite.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
    sprite.Get_Shader ()->uni3f ("mdiffuse", 0.6f, 0.6f, 0.6f);
    sprite.Get_Shader ()->uni3f ("mspecular", 1.0f, 1.0f, 1.0f);
    sprite.Get_Shader ()->uni3f ("lambient", 0.2f, 0.2f, 0.2f);
    sprite.Get_Shader ()->uni3f ("ldiffuse", 0.6f, 0.6f, 0.6f);
    sprite.Get_Shader ()->uni3f ("lspecular", 1.0f, 1.0f, 1.0f);
    sprite.Get_Shader ()->uni1f ("shininess", shininess);
    sprite.Get_Shader ()->Disable ();
  }


  void Scene_SplashScreens::Draw ()
  {
    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sprite.Get_Shader ()->Use ();
    sprite.Draw ();
    sprite.Get_Shader ()->Disable ();

    glEnable (GL_DEPTH_TEST);
    glDisable (GL_BLEND);
  }


  void Scene_SplashScreens::Change_Size (int w, int h)
  {
  }

}