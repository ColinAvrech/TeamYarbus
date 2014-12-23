///******************************************************************************/
///*!
//\file   Scene0.cpp
//\author Manas Sudhir Kulkarni
//\par    Course: GAM200
//\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
//\brief  
//*/
///******************************************************************************/
//

#include <Precompiled.h>
//#include "Scene_SplashScreens.h"
//#include "ComponentInclude.h"
//#include "ResourceManager.h"
//#include "SceneManager.h"
//#include "EventSystem.h"
//#include "BaseEvent.h"
//#include "KeyEvent.h"
//
//namespace Framework
//{
//  enum State
//  {
//    DRAW_DP,
//    DRAW_TEAM,
//    DRAW_VECTOR,
//    LOAD_NEXT,
//  };
//
//  enum Transition
//  {
//    IDLE,
//    FADE_IN,
//    FADE_OUT,
//  };
//
//  State state;
//  Transition transition;
//  //namespace Splash_Screen
//  //{
//  static float alpha = 0.0f;
//  static Transform light (NULL);
//  static Sprite teamLogo (NULL), dpLogo (NULL), vectorLogo (NULL), teamName (NULL);
//  static float camScrollSpeed = 0.05f;
//  static const float minLight = 200.0f;
//  static float shininess = minLight;
//  static const float maxLight = 3.0f;
//  static bool isPressed = false;
//  static bool drawTeamLogo = false;
//  static VAO* vao;
//  static VBO* vbo;
//  static EBO* ebo;
//  static vec2 texoffset (0.0f, 0.0f);
//  static const int samples = 4;
//  static bool fadeOut;
//  static int frames;
//  static const float rate = 1.f;
//  static const float blendin = 0.0015f;
//
//  // Constructor
//  Scene_SplashScreens::Scene_SplashScreens ()
//  {
//    //Load_Scene (filename);
//  }
//
//  // Destructor
//  Scene_SplashScreens::~Scene_SplashScreens ()
//  {
//    delete vao;
//    delete vbo;
//    delete ebo;
//  }
//
//
//  void Update_Lighting (Sprite s)
//  {
//    s.Get_Shader ()->Use ();
//    // Update Uniform Variables
//    s.Get_Shader ()->uni3fv ("lightPos", glm::value_ptr (light.GetPosition ()));
//    s.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
//    s.Get_Shader ()->uni3f ("mambient", 0.2f, 0.2f, 0.2f);
//    s.Get_Shader ()->uni3f ("mdiffuse", 0.6f, 0.6f, 0.6f);
//    s.Get_Shader ()->uni3f ("mspecular", 1.0f, 1.0f, 1.0f);
//    s.Get_Shader ()->uni3f ("lambient", 0.2f, 0.2f, 0.2f);
//    s.Get_Shader ()->uni3f ("ldiffuse", 0.6f, 0.6f, 0.6f);
//    s.Get_Shader ()->uni3f ("lspecular", 1.0f, 1.0f, 1.0f);
//    s.Get_Shader ()->uni1f ("shininess", shininess);
//    s.Get_Shader ()->Disable ();
//  }
//
//  static void OnKey_Pressed(GameObject* go, KeyEvent* key)
//  {
//    //if (state == GLFW_REPEAT)
//    switch (key->KeyValue)
//    {
//    case GLFW_KEY_W:
//      light.Translate (0, 0.01f, 0);
//      //SoundName.test2->SetPause(true);
//      break;
//    case GLFW_KEY_S:
//      light.Translate (0, -0.01f, 0);
//      //SoundName.test2->SetPause(false);
//      break;
//    case GLFW_KEY_D:
//      light.Translate (0.01f, 0, 0);
//      break;
//    case GLFW_KEY_A:
//      light.Translate (-0.01f, 0, 0);
//      break;
//    case GLFW_KEY_Q:
//      shininess += 0.5f;
//      break;
//    case GLFW_KEY_E:
//      shininess -= 0.5f;
//      break;
//    case GLFW_KEY_LEFT:
//      //////////////////////////////////////////////////////////////////////////
//      // Only For Testing
//      teamLogo.modelMatrix = glm::translate (teamLogo.modelMatrix, vec3 (-0.1f, 0.0f, 0.0f));
//      //////////////////////////////////////////////////////////////////////////
//      //camera.UpdatePosition (vec3 (-camScrollSpeed, 0.0f, 0.0f));
//      break;
//    case GLFW_KEY_RIGHT:
//      //////////////////////////////////////////////////////////////////////////
//      // Only Fore Testing
//      teamLogo.modelMatrix = glm::translate (teamLogo.modelMatrix, vec3 (0.1f, 0.0f, 0.0f));
//      //////////////////////////////////////////////////////////////////////////
//      //camera.UpdatePosition (vec3 (camScrollSpeed, 0.0f, 0.0f));
//      break;
//    case GLFW_KEY_UP:
//      Camera::current->UpdatePosition (vec3 (0.0f, camScrollSpeed, 0.0f));
//      break;
//    case GLFW_KEY_DOWN:
//      Camera::current->UpdatePosition (vec3 (0.0f, -camScrollSpeed, 0.0f));
//      break;
//    case GLFW_KEY_Z:
//      Camera::current->Zoom (0.01f);
//      break;
//    case GLFW_KEY_X:
//      Camera::current->Zoom (-0.01f);
//      break;
//    default:
//      break;
//    }
//  }
//
//
//  /*void Scene_SplashScreens::Mouse_Button(int button, int state, int mods)
//  {
//  if (state == GLFW_PRESS)
//  switch (button)
//  {
//  case GLFW_MOUSE_BUTTON_1:
//  isPressed = true;
//  std::cout << "LMB Pressed\n";
//  break;
//  default:
//  break;
//  }
//  else
//  {
//  isPressed = false;
//  }
//  }
//
//
//  void Scene_SplashScreens::Mouse_Moved(double xPos, double yPos)
//  {
//  if (isPressed)
//  {
//  camera.MouseUpdate (vec2 (float (xPos), float (yPos)));
//  }
//  }*/
//
//
//  void Scene_SplashScreens::Load_Scene (const char* filename)
//  {
//    EVENTSYSTEM->Connect (NULL, Events::KEY_ANY, BaseEvent::BaseCall (OnKey_Pressed));
//    state = DRAW_DP;
//    transition = FADE_IN;
//    fadeOut = false;
//    light.Translate (0, 0.0f, 0);
//    // Generate a Quad for Us
//    ShapeData quad = ShapeGenerator::Generate_Quad ();
//    vao = new VAO ();
//    vbo = new VBO (quad.vbo_size (), quad.vertices);
//    ebo = new EBO (quad.ebo_size (), quad.indices);
//
//    // Free Allocated Memory
//    quad.Clean ();
//
//    // Create Sprite
//    teamLogo.Create_Sprite
//      (
//      Resources::RS->Get_Shader ("LightingAnimation"),
//      Resources::RS->Get_SpriteSheet ("HighResLogo.jpg")
//      );
//    dpLogo.Create_Sprite
//      (
//      Resources::RS->Get_Shader ("Lighting"),
//      Resources::RS->Get_Texture ("DigiPenLogo.png")
//      );
//    vectorLogo.Create_Sprite
//      (
//      Resources::RS->Get_Shader ("Default"),
//      Resources::RS->Get_Texture ("VectorLogo.jpg")
//      );
//    teamName.Create_Sprite
//      (
//      Resources::RS->Get_Shader ("BlendIn"),
//      Resources::RS->Get_Texture ("TeamYarbus.png")
//      );
//
//    teamLogo.modelMatrix = glm::translate (vec3 (0.0f, 0.0f, -1.0f));
//    teamLogo.modelMatrix = glm::scale (teamLogo.modelMatrix, vec3 (0.5f, 0.5f, 1.0f));
//
//    vectorLogo.modelMatrix = glm::translate (vec3 (0.0f, 0.0f, -1.0f));
//    vectorLogo.modelMatrix = glm::scale (vectorLogo.modelMatrix, vec3 (0.5f, 0.5f, 1.0f));
//
//    teamName.modelMatrix = glm::translate (vec3 (0.0f, -0.3f, -1.0f));
//    teamName.modelMatrix = glm::scale (teamName.modelMatrix, vec3 (0.4f, 0.4f / teamName.Get_Texture()->Get_Aspect_Ratio(), 1.0f));
//
//    dpLogo.modelMatrix = glm::translate (vec3 (0, 0.0f, -1));
//    dpLogo.modelMatrix = glm::scale (dpLogo.modelMatrix, vec3 (0.5f, 0.5f / dpLogo.Get_Texture ()->Get_Aspect_Ratio (), 1.0f));
//
//    //SoundName.test1 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music2.mp3");
//    //SoundName.test1->Play();
//    //
//    //SoundName.test2 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music.mp3");
//    //SoundName.test2->Play();
//  }
//
//
//  void Scene_SplashScreens::Update (const double dt)
//  {
//    light.UpdateMatrices ();
//
//    std::cout << shininess << std::endl;
//
//    //////////////////////////////////////////////////////////////////////////
//    // STATE MACHINE FOR SCENE SPLASH SCREENS
//    //////////////////////////////////////////////////////////////////////////
//    switch (state)
//    {
//
//      //////////////////////////////////////////////////////////////////////////
//      // STATE DP LOGO
//      //////////////////////////////////////////////////////////////////////////
//    case Framework::DRAW_DP:
//      switch (transition)
//      {
//      case Framework::FADE_IN:
//        shininess -= rate;
//        if (shininess <= maxLight)
//          transition = IDLE;
//        break;
//      case Framework::IDLE:
//        ++frames;
//        if (frames > 60)
//          transition = FADE_OUT;
//        break;
//      case Framework::FADE_OUT:
//        shininess += rate;
//        if (shininess >= minLight)
//        {
//          state = DRAW_TEAM;
//          light.Translate (0.f, 0.36f, 0.f);
//          shininess = minLight;
//          frames = 0;
//          transition = FADE_IN;
//        }
//        break;
//      default:
//        break;
//      }
//      Update_Lighting (dpLogo);
//      break;
//
//      //////////////////////////////////////////////////////////////////////////
//      // STATE TEAM LOGO
//      //////////////////////////////////////////////////////////////////////////
//    case Framework::DRAW_TEAM:
//      switch (transition)
//      {
//      case Framework::FADE_IN:
//        alpha += blendin;
//        shininess -= rate;
//        if (shininess <= maxLight)
//          transition = IDLE;
//        break;
//      case Framework::IDLE:
//        alpha += blendin;
//        ++frames;
//        if (frames > 180)
//          transition = FADE_OUT;
//        break;
//      case Framework::FADE_OUT:
//        alpha -= blendin * 2;
//        shininess += rate;
//        if (shininess >= minLight)
//        {
//          frames = 0;
//          state = LOAD_NEXT;
//          transition = IDLE;
//        }
//        break;
//      default:
//        break;
//      }
//      Update_Lighting (teamLogo);
//      break;
//
//    case Framework::DRAW_VECTOR:
//      switch (transition)
//      {
//      case Framework::IDLE:
//        ++frames;
//        if (frames > 120)
//          state = LOAD_NEXT;
//        break;
//      case Framework::FADE_IN:
//        break;
//      case Framework::FADE_OUT:
//        break;
//      default:
//        break;
//      }
//      break;
//
//      //////////////////////////////////////////////////////////////////////////
//      // STATE LOAD NEXT SCENE
//      //////////////////////////////////////////////////////////////////////////
//    case Framework::LOAD_NEXT:
//      SCENEMANAGER->Load_Next_Scene ();
//      break;
//    default:
//      break;
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////////////////////////
//  }
//
//
//  void Scene_SplashScreens::Draw ()
//  {
//    glClearColor (0, 0, 0, 0);
//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glDisable (GL_DEPTH_TEST);
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//
//    switch (state)
//    {
//    case Framework::DRAW_DP:
//      dpLogo.Draw ();
//      break;
//
//    case Framework::DRAW_TEAM:
//      teamLogo.Draw ();
//      teamName.Get_Shader ()->Use ();
//      teamName.Get_Shader ()->uni1f ("alpha", alpha);
//      teamName.Draw ();
//      break;
//
//    case Framework::DRAW_VECTOR:
//      vectorLogo.Draw ();
//      break;
//
//    case Framework::LOAD_NEXT:
//      break;
//    default:
//      break;
//    }
//
//    glEnable (GL_DEPTH_TEST);
//    glDisable (GL_BLEND);
//  }
//
//
//  void Scene_SplashScreens::Change_Size (int w, int h)
//  {
//    glViewport (0, 0, w, h);
//    Camera::main->viewToProjection = glm::perspective (70.0f, (float) w / h, 0.1f, 10.0f);
//  }
//
//}