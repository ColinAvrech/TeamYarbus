/******************************************************************************/
/*!
\file   UISystem.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  UI system for Cinder engine
*/
/******************************************************************************/

#pragma once


namespace Framework
{
  class UILayer;

  class UISystem : public BaseSystem
  {
  public:
    UISystem();
    ~UISystem();
    //!Initialize the system
    virtual bool Initialize();

    // Called every frame
    virtual void Update(const float& dt);

    virtual bool UpdatesOnPaused();

    virtual const string GetName() { return "UISystem"; }

    void AddLayer(const char *layername);

  private:
    std::vector<UILayer*> UILayers;
    void Clear();
  };

  extern UISystem* UISYSTEM;
}
