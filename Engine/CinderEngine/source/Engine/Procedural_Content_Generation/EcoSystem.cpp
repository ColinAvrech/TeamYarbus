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

META_DEFINE( Framework::EcoSystem, EcoSystem )
{

}


namespace Framework
{ 
	void EcoSystem::Serialize(Serializer::DataNode* data)
	{

	}

	void EcoSystem::Initialize()
	{
    Terrain2D* terrainComp = static_cast<Terrain2D*>(gameObject->GetComponent("Terrain2D"));
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
		ErrorIf(!terrain, "Can't have no trees with no ground, YO!");
    Initialize_Archetype_List();
    terrain = terrainComp->GetTerrain();
    water = terrainComp->GetWater();
    MapWidth = terrainComp->GetWidth();
    MapHeight = terrainComp->GetPeakHeight();
		//Set up barren ground
		tree_list = new int[MapWidth];
		for (int i = 0; i < MapWidth; ++i)
			tree_list[i] = int(OPEN);

		Translation = tform->GetPosition();
		Scale = tform->GetScale();
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
    //if slope is too steep
    if (std::abs(terrain[pos] - terrain[pos - 1]) > 1 || std::abs(terrain[pos] - terrain[pos + 1]) > 1)
      return OPEN;
    else
      return INVALID;
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

      //int type = Evaluate_Compatibility(i);
      //if (type != INVALID)
        //tree_list[i] = ;
      
        //Hack!!
      if (i % 8 == 0)
      {
        GameObject *newobj;
        newobj = OBJECTSYSTEM->LoadArchetype("Tree03D.Archetype");
        static_cast<Transform*>(newobj->GetComponent("Transform"))->Translate(offsetX + Translation.x, offsetY + Translation.y, Translation.z);
      }
      //if (tree_list[i] != OPEN)
      /*if (i % (rand() % 10 + 2) == 0 && terrain[i] <= 72)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_0, ID
        );
      else if(rand() % 2)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_SHORT_GRASS, ID
        );*/
      /*else if (rand() % 2)
        GenerateType(
        offsetX + Translation.x,
        offsetY + Translation.y,
        Translation.z,
        TREE_TALL_GRASS, ID
        );*/
		}
	}

	void EcoSystem::GenerateType(float x, float y, float z, Tree_Type type, unsigned ID)
	{
    GameObject *newobj;
    
    newobj = OBJECTSYSTEM->LoadArchetype(Archetype_List[type]);

    static_cast<Transform*>(newobj->GetComponent("Transform"))->Translate(x, y, z);

    FireGroup* fsm = nullptr;
    fsm = static_cast<FireGroup*>(newobj->GetComponent("FireGroup"));
    if (fsm != nullptr && fsm->firePoints.size())
    {
      Physics::THERMODYNAMICS->Add_Group(fsm);
    }
	}
}

