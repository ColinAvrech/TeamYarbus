/*****************************************************************************/
/*!
\file   Trees.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Procedurally generate trees of different types.
*/
/*****************************************************************************/

#pragma once
#include "Component.h"
#include "Tree2D.h"

namespace Framework
{
	////////////////////////////////////////////////////////////
	//      ECO SYSTEM
	////////////////////////////////////////////////////////////
	class EcoSystem : public Component
	{
	public:

		/*-----------------------------------------------------------------------
		// Constructors
		-----------------------------------------------------------------------*/
#pragma region Constructors

		EcoSystem(){};

		EcoSystem(GameObject *obj){ gameObject = obj; }


#pragma endregion
		/*-----------------------------------------------------------------------
		// Static Public Variables
		-----------------------------------------------------------------------*/
#pragma region Static Public Variables



#pragma endregion


		/*-----------------------------------------------------------------------
		// Static Public Functions
		-----------------------------------------------------------------------*/
#pragma region Static Public Functions

#pragma endregion


		/*-----------------------------------------------------------------------
		// Destructor
		-----------------------------------------------------------------------*/
#pragma region Destructors
		// ~EcoSystem();

#pragma endregion


		/*-----------------------------------------------------------------------
		// Public Variables
		-----------------------------------------------------------------------*/
#pragma region Public Variables

#pragma endregion

		/*---------------------------------------------------------------------*/
		// Public Structs
		/*---------------------------------------------------------------------*/
#pragma region Public Structs

#pragma endregion

		/*-----------------------------------------------------------------------
		// Public Functions
		-----------------------------------------------------------------------*/
#pragma region Public Functions
		//!Initialize the system
		virtual void Initialize();
		//Serialize Component
		virtual void Serialize(Serializer::DataNode* data);

		// Called every frame
		void Update(const float &dt);

		int GetTotalObjects();

	private:

		/*-----------------------------------------------------------------------
		// Private Variables
		-----------------------------------------------------------------------*/
#pragma region Private Variables
		const float *terrain;
		const float *water;
		int *tree_list;
		std::pair<int, int>Min_Space; //Implement later
		int MapWidth;
    int MapHeight;
		glm::vec3 Translation;
		glm::vec3 Scale;
		int Total_Objects = 0;
    std::unordered_map<Tree_Type, const char *> Archetype_List;
#pragma endregion


		/*-----------------------------------------------------------------------
		// Private Structs
		-----------------------------------------------------------------------*/
#pragma region Private Structs


#pragma endregion

		/*-----------------------------------------------------------------------
		// Private Functions
		-----------------------------------------------------------------------*/
#pragma region Private Functions
		void GenerateVegetation();

		void GenerateType(float x, float y, float z, Tree_Type type, unsigned ID);

		int Evaluate_Compatibility(int pos);

    void Initialize_Archetype_List();
#pragma endregion


		/*-----------------------------------------------------------------------
		// Static Functions
		-----------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion

	}; //class thermodynamics

}  //Framework

