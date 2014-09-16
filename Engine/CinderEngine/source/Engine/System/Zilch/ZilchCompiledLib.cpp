#include "ZilchCompiledLib.h"

namespace Framework
{
  namespace ScriptSystem
  {    
    bool ScriptSystem::Initialize()
    {
      // Any one time startup and static initialization Zilch needs to do
      // This also registers a custom assertion handler (Zilch code has many user friendly asserts!)
      Zilch::ZilchStartup(Zilch::Debugging::UseZilchErrorHandler);
      // Here, we can register our own callback for when compilation errors occur
      // The default callback prints the file, line/character number, and message to stderr
      Errors.AddCallback(Zilch::DefaultErrorCallback, nullptr);
      // A project contains all of the code we combine together to make a single Zilch library
      Zilch::Project project(Errors); //declared in init function. Loops through till all scripts are compiled
      
      //Test Code----------------------------------------------------------------------------
      project.AddCodeFromFile("test.z");

      // Create a list of dependent libraries, in our case we're really not adding anything to this
      // A side note: the Core library in Zilch is always added as a dependency, because Core includes
      // things like Integer, Boolean, Real, the basic vector types, String, etc
      Zilch::Module dependencies(Errors);

      // The 'Compile' function actually returns a Zilch Library
      // Be careful! If the code fails to compile, this function will return null
      Zilch::LibraryRef library = project.Compile("Test", dependencies, Zilch::EvaluationMode::Project);
      ErrorIf(library == nullptr, "Failed to compiler library");

      // We want to link together all the libraries that we depended upon, along with our own library
      dependencies.AddLibrary(library);

      // Link all the libraries together into one ExecutableState
      LinkedLibs = dependencies.Link();
      ErrorIf(LinkedLibs == nullptr, "Failed to link libraries together");

      // Find the Player type we created in Zilch
      Zilch::BoundType* player = library->BoundTypes.findValue("Player", nullptr);
      ErrorIf(player == nullptr, "Failed to find a Zilch type named 'Player'");

      // Find the static SayHello function we created on the Player type
      // We pass in an array of types to specify the arguments we want, in this case its an empty array
      // We also pass in the void type because we don't expect a return value
      Zilch::Function* sayHello = player->FindFunction("SayHello", Zilch::Array<Zilch::Type*>(), ZilchTypeId(void), Zilch::FindMemberOptions::None);
      ErrorIf(sayHello == nullptr, "Failed to find function 'SayHello' on Zilch type 'Player'");

      // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
      Zilch::Console::AddWriteTextCallback(Zilch::DefaultWriteText, nullptr);

      // We can also setup the console so that any 'Read' functions will attempt to read from stdin
      Zilch::Console::SetReadTextCallback(Zilch::DefaultReadText, nullptr);

      // The exception report stores any exceptions that may have occurred while executing code
      // Exceptions include accessing arrays out of bounds, dereferencing null, etc
      Zilch::ExceptionReport report;

      // The call object uses its constructor to initialize a stack frame in
      // the ExecutableState, and removes that stack frame inside its destructor
      // It is important to wrap this in a scope to ensure destruction happens when we intend it to

      // Player.SayHello()
      {
        // Invoke the SayHello function, which should print out to the console
        Zilch::Handle handle = LinkedLibs->AllocateDefaultConstructedHeapObject(player, report, Zilch::HeapFlags::ReferenceCounted);
        Zilch::Call call(sayHello, LinkedLibs);
        call.SetHandle(Zilch::Call::This, handle);
        call.Invoke(report);
      }


      //Test code end
      return true;
    }
    
    void ScriptSystem::Update(const double dt)
    {
      
    }

    ScriptSystem::~ScriptSystem()
    {
      delete LinkedLibs;
      Zilch::ZilchShutdown();
    }
  }
}