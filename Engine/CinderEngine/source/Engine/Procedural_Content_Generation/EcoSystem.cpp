/*****************************************************************************/
/*!
\file   EcoSystem.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate trees of different types.
*/
/*****************************************************************************/

#include <Precompiled.h>
#include "Trees.h"
#include "GameObject.h"
#include "Terrain2d.h"
#include "Tree2D.h"
#include "ObjectSystem.h"
#include "FireStarter.h"
#include "AudioComponent.h"
#include "Thermodynamics.h"

namespace Framework
{ 
	void EcoSystem::Serialize(Serializer::DataNode* data)
	{

	}

	void EcoSystem::Initialize()
	{
		ErrorIf(!gameObject->Terrain2D, "Can't have no trees with no ground, YO!");
    Initialize_Archetype_List();
		terrain = gameObject->Terrain2D->GetTerrain();
		water = gameObject->Terrain2D->GetWater();
		MapWidth = gameObject->Terrain2D->GetWidth();
    MapHeight = gameObject->Terrain2D->GetPeakHeight();
		//Set up barren ground
		tree_list = new int[MapWidth];
		for (int i = 0; i < MapWidth; ++i)
			tree_list[i] = int(OPEN);

		Translation = gameObject->Transform->GetPosition();
		Scale = gameObject->Transform->GetScale();
		GenerateVegetation();
	}

  void EcoSystem::Initialize_Archetype_List()
  {
    Archetype_List[TREE_0] = "Tree0.Archetype";
    Archetype_List[TREE_1] = "Tree1.Archetype";
    Archetype_List[TREE_2] = "Tree2.Archetype";
    Archetype_List[TREE_3] = "Tree3.Archetype";
    Archetype_List[TREE_4] = "Tree4.Archetype";
    Archetype_List[TREE_5] = "Tree5.Archetype";
    Archetype_List[TREE_SHORT_GRASS] = "TreeGrass.Archetype";
    Archetype_List[TREE_TALL_GRASS] = "TreeReed.Archetype";
  }

	int EcoSystem::Evaluate_Compatibility(int pos)
	{
		//int i;
		//for ( i = pos; i >= 0 || tree_list[i] == OPEN || tree_list[i] == GRASS; --i);

		//int AvailableSpace = pos - i;
		////HACK!!!
		//if (AvailableSpace > 3 && water[pos] > 0.5f && water[pos] < 0.7f)
		//  return TREE_5;

		//else if (AvailableSpace <= 3 && water[pos] > 0.1f && water[pos] < 0.9f)
		//  return GRASS;

		//else
		//  return OPEN;
		return 0;
	}

	int EcoSystem::GetTotalObjects()
	{
		return Total_Objects;
	}

	void EcoSystem::GenerateVegetation()
	{
		float nX = 2.f * Scale.x / (MapWidth - 1);
		float nY = 2.f * Scale.y / (MapHeight - 1);
		float offsetX = -Scale.x;
		float offsetY = nY;
		float previousHeight = -1.f;
		unsigned int ID = OBJECTSYSTEM->GameObjects.size() + 1;
    for (int i = 0; i < MapWidth; ++i, ++ID)
    {
      offsetY = (terrain[i] * nY) / 2.f;
      if (offsetY < 0)
        offsetY = 0.0f;
      offsetX += nX;

      //tree_list[i] = Evaluate_Compatibility(i);
      //Hack!!
      //if (tree_list[i] != OPEN)
      if (i % (rand() % 6 + 2) == 0 && terrain[i] <= 72)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_3, ID
        );
      else if(rand() % 2)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_SHORT_GRASS, ID
        );
      else if (rand() % 2)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_0, ID
        );
		}
	}

	void EcoSystem::GenerateType(float x, float y, float z, Tree_Type type, unsigned ID)
	{
    GameObject *newobj;
    
    newobj = OBJECTSYSTEM->LoadArchetype(Archetype_List[type]);

		newobj->Transform->Translate(x, y, z);

    FireGroup* fsm = nullptr;
    fsm = newobj->FireGroup;
    if (fsm != nullptr && fsm->firePoints.size())
    {
      Physics::THERMODYNAMICS->Add_Group(fsm);
    }
	}
}

