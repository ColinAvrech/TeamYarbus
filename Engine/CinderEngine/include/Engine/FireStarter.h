/******************************************************************************/
/*!
\file   GameLogic.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/

#ifndef _FIRE_STARTER_H
#define _FIRE_STARTER_H

#include "Component.h"
#include "DataNode.h"
#include "TDLib.h"
#include "Grid2D.h"

namespace Framework
{
  using namespace Physics;
  class FireStarter;
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

  class FireStarter : public Component
  {
  public:
    FireStarter();
    ~FireStarter(){}
    void LightOnFire();
    void DouseFire();
    void Update(const double dt);
    void Serialize(Serializer::DataNode* data);
    void Initialize();
    int material_type;

    const static std::string Name;
    bool onFire;	
    float Fuel;
    float initTemp;
    FireGrid grid;
  };
}

#endif _FIRE_STARTER_H