/******************************************************************************/
/*!
\file   WindowSystem.h
\author Micah Rust, Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/

#include <Precompiled.h>

#pragma warning(disable:4413)

namespace Framework
{

  ZilchDefineType(ZilchComponent, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	  ZilchBindConstructor();
	  ZilchBindDestructor();
	  ZilchBindMethod(Create);
	  ZilchBindMethod(Serialize);
	  ZilchBindMethod(Initialize);
  }

  //CONSTRUCTOR
  ZilchComponent::ZilchComponent()
  {
    
  }


  ZilchComponent::~ZilchComponent()
  {
	  
  }

  void ZilchComponent::Create(GameObject* owner)
  {
	  gameObject = owner;
	  std::cout << "CREATING ZILCH COMPONENT" << std::endl;
  }

  void ZilchComponent::Serialize(Serializer::DataNode* owner)
  {
	  std::cout << "SERIALIZING ZILCH COMPONENT" << std::endl;
	  //Serializer::DataNode* temp = componentData->FindElement(componentData, "OHai");
	  //temp->GetValue(&ohai);
  }

  //INITIALIZE
  void ZilchComponent::Initialize()
  {
	  std::cout << "INITIALIZING ZILCH COMPONENT" << std::endl;


	  /*
	  //Get pointer to appropriate library
    library = &(ZILCH->lib);
    //Bind the Zilch class
    ZilchClass = (*library)->BoundTypes.findValue(ZilchClassName->c_str(), nullptr);
    ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", ZilchClassName);

    //Create an array of arguments
    Zilch::Array<Zilch::Type*> args;
	//args.push_back(ZilchTypeId(ObjectSystem*));
	args.push_back(ZilchTypeId(GameObject*));
    //Find the Initialize function
    // We pass in an array of types to specify the arguments we want, in this case its an empty array
    // We also pass in the void type because we don't expect a return value
    ZilchInitialize = ZilchClass->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
    ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ZilchClass);

    // The exception report stores any exceptions that may have occurred while executing code
    // Exceptions include accessing arrays out of bounds, dereferencing null, etc
    Zilch::ExceptionReport report;
    Zilch::ExecutableState* state = ZILCH->GetDependencies();
    ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);
	*/
	//var Owner : GameObject;
	//owner->PropertyType == ZilchTypeId(Real);
	/*
	Field* owner = ZilchClass->InstanceFields["Owner"];
	
	{
		Zilch::Call call(owner->Set, ZILCH->GetDependencies());
		call.SetHandle(Zilch::Call::This, bob);
		call.Set(0, gameObject);
	}
	*/
	//Trevor touched this stuff
	//this->gameObject = (GameObject*)0x12345678;

  }
   
}