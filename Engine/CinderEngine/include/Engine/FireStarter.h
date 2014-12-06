/******************************************************************************/
/*!
\file   FireStarter.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#ifndef _FIRE_STARTER_H
#define _FIRE_STARTER_H

#include "Component.h"
#include "GameObject.h"
#include "DataNode.h"
#include "TDLib.h"
#include "Grid2D.h"
#include <vector>

using std::vector;

namespace Framework
{

  using namespace Physics;
  class FireStarter;
  /*
  class FireGrid
  {
  public:
    void Create (FireStarter* transform);
    void Destroy ();
    void Update ();
    void Draw ();
    Grid2D <float> temperatures;
    Grid2D <glm::vec2> positions;
  };
  */

  class FireStarterManager;
  class FireStarter
  {
    public:
      FireStarter(const vec2& pos, FireStarterManager* fsm);
      ~FireStarter(){}
      void LightOnFire();
      void DouseFire();
      void Update(const double dt);
      vec2 GetPosition();
      vec2 GetGridPosition();


      int material_type;

      vec2 positionOffset;
      bool onFire;	
      float Fuel;
      float initTemp;
      //FireGrid grid;
      FireStarterManager* manager;
  };

  class FireStarterManager : public Component
  {
  public:
    FireStarterManager();
    ~FireStarterManager();

    virtual void Initialize(){ gameObject->FireStarterManager = this; }
    virtual void Serialize(Serializer::DataNode* data);
    void AddFireStarter(FireStarter* newFirePoint);

    const static std::string Name;
    bool onFire;
    static int numTreesLeft;
    static int numTreesStart;
    int numPoints = 0;
  //private:
    vector<const FireStarter *> firePoints;
  };
}

#endif _FIRE_STARTER_H