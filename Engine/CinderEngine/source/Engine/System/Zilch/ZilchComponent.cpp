#include "ScriptComponent.h"
#include "ZilchCompiledLib.h"

#pragma warning(disable:4413)

namespace Framework
{
  //CONSTRUCTOR
  ZilchComponent::ZilchComponent(std::string& name)
  {
    ZilchClassName = new Zilch::String(name.c_str());
  }

  //INITIALIZE
  void ZilchComponent::Initialize()
  {
    //Get pointer to appropriate library
    library = &(ZILCH->lib);
    //Bind the Zilch class
    ZilchClass = (*library)->BoundTypes.findValue(ZilchClassName->c_str(), nullptr);
    ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", ZilchClassName);

    //Create an array of arguments
    Zilch::Array<Zilch::Type*> args;

    //Find the Initialize function
    // We pass in an array of types to specify the arguments we want, in this case its an empty array
    // We also pass in the void type because we don't expect a return value
    ZilchInitialize = ZilchClass->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
    ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ZilchClass);

    // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
    Zilch::Console::AddWriteTextCallback(Zilch::DefaultWriteText, nullptr);

    // We can also setup the console so that any 'Read' functions will attempt to read from stdin
    Zilch::Console::SetReadTextCallback(Zilch::DefaultReadText, nullptr);

    // The exception report stores any exceptions that may have occurred while executing code
    // Exceptions include accessing arrays out of bounds, dereferencing null, etc
    Zilch::ExceptionReport report;
    Zilch::Handle bob;
    Zilch::ExecutableState* state = ZILCH->GetDependencies();
    bob = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);

    {
      // Invoke the SayHello function, which should print out to the console
      Zilch::Call call(ZilchInitialize, ZILCH->GetDependencies());
      call.SetHandle(Zilch::Call::This, bob);
      //call.Set<Zilch::String>(0, ohai.c_str());
      //call.SetHandle(1, engineHandle);
      call.Invoke(report);
    }

    bob.Delete();
  }

  void ZilchComponent::Serialize(Serializer::DataNode* componentData)
  {
    Serializer::DataNode* temp = componentData->FindElement(componentData, "OHai");
    temp->GetValue(&ohai);
  }
   
}