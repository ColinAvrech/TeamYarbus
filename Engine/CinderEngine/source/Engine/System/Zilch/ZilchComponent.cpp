#include "ScriptComponent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{
  namespace ScriptSystem
  {
    //CONSTRUCTOR
    ZilchComponent::ZilchComponent(std::string* name)
      : ZilchClass(Zero::String(name->c_str()))
    {
      //Do Stuff
    }

    //INITIALIZE
    bool ZilchComponent::Initialize()
    {
      //Get pointer to appropriate library
      //library = SCRIPTSYSTEM->GetZilchLib(ZilchClass.c_str());
      //Bind the Zilch class
      This = (*library)->BoundTypes.findValue(ZilchClass, nullptr);
      ErrorIf(This == nullptr, "Failed to find a Zilch type named ", ZilchClass);

      //Create an array of arguments
      Zilch::Array<Zilch::Type*> args;

      //Find the Initialize function
      // We pass in an array of types to specify the arguments we want, in this case its an empty array
      // We also pass in the void type because we don't expect a return value
      ZilchInitialize = This->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
      ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ZilchClass);

      // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
      Zilch::Console::AddWriteTextCallback(Zilch::DefaultWriteText, nullptr);

      // We can also setup the console so that any 'Read' functions will attempt to read from stdin
      Zilch::Console::SetReadTextCallback(Zilch::DefaultReadText, nullptr);

      

      return true;
    }
   
  }
}