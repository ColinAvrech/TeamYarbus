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
  enum State
  {
    DRAW_DP,
    DRAW_TEAM,
    LOAD_NEXT,
  };

  enum Transition
  {
    IDLE,
    FADE_IN,
    FADE_OUT,
  };

  State state;
  Transition transition;
  //namespace Splash_Screen
  //{
  static Camera camera(true);
  static Transform light;
  static Sprite sprite;
  static Sprite sprite1;
  static float camScrollSpeed = 0.05f;
  static const float minLight = 200.0f;
  static float shininess = minLight;
  static const float maxLight = 3.0f;
  static bool isPressed = false;
  static bool drawTeamLogo = false;
  static VAO* vao;
  static VBO* vbo;
  static EBO* ebo;
  static glm::vec2 texoffset (0.0f, 0.0f);
  static const int samples = 4;
  static bool fadeOut;
  static int frames;
  static const float rate = 1.f;

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


  void Update_Lighting (Sprite s)
  {
    s.Get_Shader ()->Use ();
    // Update Uniform Variables
    s.Get_Shader ()->uni3fv ("lightPos", glm::value_ptr (light.GetPosition ()));
    s.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
    s.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
    s.Get_Shader ()->uni3f ("mdiffuse", 0.6f, 0.6f, 0.6f);
    s.Get_Shader ()->uni3f ("mspecular", 1.0f, 1.0f, 1.0f);
    s.Get_Shader ()->uni3f ("lambient", 0.2f, 0.2f, 0.2f);
    s.Get_Shader ()->uni3f ("ldiffuse", 0.6f, 0.6f, 0.6f);
    s.Get_Shader ()->uni3f ("lspecular", 1.0f, 1.0f, 1.0f);
    s.Get_Shader ()->uni1f ("shininess", shininess);
    s.Get_Shader ()->Disable ();
  }

  void Scene_SplashScreens::Key_Pressed(int key, int scanCode, int action, int mods)
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


  void Scene_SplashScreens::Mouse_Button(int button, int state, int mods)
{
    if (state == GLFW_PRESS)
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


  void Scene_SplashScreens::Mouse_Moved(double xPos, double yPos)
{
    if (isPressed)
    {
      camera.MouseUpdate (glm::vec2 (float (xPos), float (yPos)));
    }
  }


  void Scene_SplashScreens::Load_Scene (const char* filename)
  {
    state = DRAW_DP;
    transition = FADE_IN;
    fadeOut = false;
    light.Translate (0, 0.0f, 0);
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
      Resources::RS->Get_Texture ("DigiPenLogo.png")
      );

    sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    sprite.modelMatrix = glm::scale (sprite.modelMatrix, glm::vec3 (0.5f, 0.5f, 1.0f));

    sprite1.modelMatrix = glm::translate (glm::vec3 (0, 0.0f, -1));
    sprite1.modelMatrix = glm::scale (sprite1.modelMatrix, glm::vec3 (0.5f, 0.5f / sprite1.Get_Texture ()->Get_Aspect_Ratio (), 1.0f));

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

    //////////////////////////////////////////////////////////////////////////
    // STATE MACHINE FOR SCENE SPLASH SCREENS
    //////////////////////////////////////////////////////////////////////////
    switch (state)
    {

      //////////////////////////////////////////////////////////////////////////
      // STATE DP LOGO
      //////////////////////////////////////////////////////////////////////////
    case Framework::DRAW_DP:
      switch (transition)
      {
      case Framework::FADE_IN:
        shininess -= rate;
        if (shininess <= maxLight)
          transition = IDLE;
        break;
      case Framework::IDLE:
        ++frames;
        if (frames > 30)
          transition = FADE_OUT;
        break;
      case Framework::FADE_OUT:
        shininess += rate;
        if (shininess >= minLight)
        {
          state = DRAW_TEAM;
          light.Translate (0.f, 0.36f, 0.f);
          shininess = minLight;
          frames = 0;
          transition = FADE_IN;
        }
        break;
      default:
        break;
      }
      Update_Lighting (sprite1);
      break;

      //////////////////////////////////////////////////////////////////////////
      // STATE TEAM LOGO
      //////////////////////////////////////////////////////////////////////////
    case Framework::DRAW_TEAM:
      switch (transition)
      {
      case Framework::FADE_IN:
        shininess -= rate;
        if (shininess <= maxLight)
          transition = IDLE;
        break;
      case Framework::IDLE:
        ++frames;
        if (frames > 30)
          transition = FADE_OUT;
        break;
      case Framework::FADE_OUT:
        shininess += rate;
        if (shininess >= minLight)
        {
          state = LOAD_NEXT;
          light.Translate (0.f, 0.36f, 0.f);
        }
        break;
      default:
        break;
      }
      Update_Lighting (sprite);
      break;

      //////////////////////////////////////////////////////////////////////////
      // STATE LOAD NEXT SCENE
      //////////////////////////////////////////////////////////////////////////
    case Framework::LOAD_NEXT:
      SCENEMANAGER->Load_Next_Scene ();
      break;
    default:
      break;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
  }


  void Scene_SplashScreens::Draw ()
  {
    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    switch (state)
    {
    case Framework::DRAW_DP:
      sprite1.Draw ();
      break;

    case Framework::DRAW_TEAM:
      sprite.Draw ();
      break;

    case Framework::LOAD_NEXT:
      break;
    default:
      break;
    }

    glEnable (GL_DEPTH_TEST);
    glDisable (GL_BLEND);
  }


  void Scene_SplashScreens::Change_Size (int w, int h)
  {
    glViewport (0, 0, w, h);
    Camera::main->viewToProjection = glm::perspective (70.0f, (float) w / h, 0.1f, 10.0f);
  }

}