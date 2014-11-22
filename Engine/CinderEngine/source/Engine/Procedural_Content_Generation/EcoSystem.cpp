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
    MapWidth = gameObject->Terrain2D->GetWidth();
    Translation = gameObject->Transform->GetPosition();
    Scale = gameObject->Transform->GetScale();
    GenerateVegetation();
  }

  void EcoSystem::GenerateVegetation()
  {
    GenerateGrass();
    //Generate other plants
  }

  void EcoSystem::GenerateGrass()
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

      GameObject* grass = new GameObject(ID);
      grass->Name = "Grass";
      OBJECTSYSTEM->GameObjects[ID] = grass;
      
      Component* c = grass->AddComponent("Transform");
      if (c)
      {
        static_cast<Transform*>(c)->Translate(offsetX + Translation.x,
                                              offsetY + Translation.y, 
                                              Translation.z);
        //static_cast<Transform*>(c)->Scale(Scale.x, Scale.y, Scale.z);
        c->Initialize();
      }
      c = grass->AddComponent("Tree2D");
      if (c)
      {
        glm::vec4 color = { 0.2f, 0.0f, 0.01f, 1.f };
        static_cast<Tree2D*>(c)->Set(color, Tree_Type::GRASS);
        c->Initialize();
      }
    }
  }
}