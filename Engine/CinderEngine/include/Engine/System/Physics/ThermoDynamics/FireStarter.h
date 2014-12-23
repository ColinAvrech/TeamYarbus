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

  class FireGroup;

  class FireStarter
  {
    public:
      FireStarter(const vec2& pos, FireGroup* fsm);
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
      FireGroup* manager;
  };

  class FireGroup : public Component
  {
  public:
    FireGroup();
    ~FireGroup();

    virtual void Initialize();

    virtual void Serialize(Serializer::DataNode* data);
    void AddFireStarter(FireStarter* newFirePoint);

    const static std::string Name;
    bool onFire;
  //private:
    vector<FireStarter *> firePoints;
  };
}

#endif _FIRE_STARTER_H