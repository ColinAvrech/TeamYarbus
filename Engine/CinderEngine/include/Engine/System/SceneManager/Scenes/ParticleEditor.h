/******************************************************************************/
/*!
\file   ParticleEditor.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PARTICLE_EDITOR_H
#define _PARTICLE_EDITOR_H

#include "Scene.h"

namespace Framework
{

  class ParticleEditor : public Scene
  {
  public:
    ParticleEditor ();
    ~ParticleEditor ();

  private:
    virtual void Key_Pressed (int key, int state, int action, int mods) override;
    virtual void Mouse_Button (int button, int state, int mods) override;
    virtual void Mouse_Moved (double xPos, double yPos) override;
    virtual void Update (const double dt) override;
    virtual void Draw () override;
    virtual void Load_Scene (const char* filename) override;
    virtual void Change_Size (int w, int h) override;
  };

}

#endif