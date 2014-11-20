/******************************************************************************/
/*!
\file   UISystem.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _UI_SYS_H
#define _UI_SYS_H

#include "BaseSystem.h"

namespace Framework
{
  class UIEvent;

  class UIListener
  {
  public:
    UIListener ();
    ~UIListener ();
    void Initialize ();
    void MessageHandler (UIEvent*);
  };


  class UISystem : public BaseSystem
  {
  public:
  UISystem ();
  ~UISystem ();
  
  virtual bool Initialize ();
  virtual void Update(const double& dt);
  virtual const std::string GetName() { return "UISystem"; }

  private:
    static UIListener listener;

  };

  extern UISystem* UISYSTEM;
}

#endif