/*****************************************************************************/
/*!
\file   EcoSystem.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate trees of different types.
*/
/*****************************************************************************/

#include "Trees.h"
#include "GameObject.h"
#include "Terrain2d.h"
#include "Tree2D.h"
#include "ObjectSystem.h"

namespace Framework
{
  void EcoSystem::Serialize(Serializer::DataNode* data)
  {

  }

  void EcoSystem::Initialize()
  {
    ErrorIf(!gameObject->Terrain2D, "Can't have no trees with no ground, YO!");
    
    terrain = gameObject->Terrain2D->GetTerrain();
    water = gameObject->Terrain2D->GetWater();
    MapWidth = gameObject->Terrain2D->GetWidth();
    //Set up barren ground
    tree_list = new int[MapWidth];
    for (int i = 0; i < MapWidth; ++i)
      tree_list[i] = int(OPEN);

    Translation = gameObject->Transform->GetPosition();
    Scale = gameObject->Transform->GetScale();
    GenerateVegetation();
  }

  int EcoSystem::Evaluate_Compatibility(int pos)
  {
    int i;
    for ( i = pos; i >= 0 || tree_list[i] == OPEN || tree_list[i] == GRASS; --i);

    int AvailableSpace = pos - i;
    //HACK!!!
    if (AvailableSpace > 3 && water[pos] > 0.5f && water[pos] < 0.7f)
      return TREE_5;

    else if (AvailableSpace <= 3 && water[pos] > 0.1f && water[pos] < 0.9f)
      return GRASS;

    else
      return OPEN;
  }

  void EcoSystem::GenerateVegetation()
  {
    float nX = 2.f * Scale.x / (MapWidth - 1);
    float nY = 2.f * Scale.y / (MapWidth - 1);
    float offsetX = -Scale.x;
    float offsetY = nY;
    float previousHeight = -1.f;
    unsigned int ID = OBJECTSYSTEM->GameObjects.size() + 1;
    for (int i = 0; i < MapWidth; ++i, ++ID)
    {
      offsetY = (terrain[i]) * nY;
      if (offsetY < 0)
        offsetY = 0.0f;
      offsetX += 4 * nX;

      tree_list[i] = Evaluate_Compatibility(i);
      //Hack!!
      if (tree_list[i] == TREE_5)
        GenerateType(
          offsetX + Translation.x,
          offsetY + Translation.y,
          Translation.z,
          TREE_5, ID
          );
      }
    }

  void EcoSystem::GenerateType(float x, float y, float z, int type, unsigned ID)
  {
    GameObject* grass = new GameObject(ID);
    grass->Name = "Grass";
    OBJECTSYSTEM->GameObjects[ID] = grass;

    Component* c = grass->AddComponent("Transform");
    if (c)
    {
      static_cast<Transform*>(c)->Translate(x, y, z);
      c->Initialize();
    }
    c = grass->AddComponent("Tree2D");
    if (c)
    {
      glm::vec4 color = { 0.2f, 0.0f, 0.01f, 1.f };
      static_cast<Tree2D*>(c)->Set(color, Tree_Type(type));
      c->Initialize();
    }
    //Add firestarter component here

  }
}

