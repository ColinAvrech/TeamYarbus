#include <Precompiled.h>
#include <dbghelp.h>
#pragma comment( lib, "dbghelp" )
#include "Profiler\SamplingProfiler.h"

namespace Framework
{
#ifdef _DEBUG
  SamplingProfiler::SamplingProfiler(const unsigned& p_maxsamples) : maxSamples(p_maxsamples)
  {
    id = ProfilesTaken++;

    full = false;
    exported = false;
    exited = false;

    workerThread = nullptr;
    mainThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, 0, GetCurrentThreadId());
    CheckForError();
    
    sampleEvent = timeSetEvent(1, 1, ProfilerCallback, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);
  }

  SamplingProfiler::~SamplingProfiler()
  {
    if (!IsExported())
      ExportResults();
    
    if (!exited)
      Exit();
  }

  void SamplingProfiler::Exit()
  {
    timeKillEvent(sampleEvent);

    if (workerThread)
    {
      if (!CloseHandle(workerThread))
        CheckForError();

      workerThread = nullptr;
    }

    if (mainThread)
    {
      if (!CloseHandle(mainThread))
        CheckForError();

      mainThread = nullptr;
    }

    exited = true;
  }

  bool SamplingProfiler::IsFull()
  {
    if (!full && maxSamples != NOMAX && !(samples.size() < maxSamples))
    {
      SetFull();
    }

    return full;
  }

  void SamplingProfiler::SetFull()
  {
    full = true;
  }

  void SamplingProfiler::SetExported()
  {
    ExportResults(); 
    exported = true;
  }

  void SamplingProfiler::TakeSample()
  {
    if (!IsFull())
    {
      if (workerThread == nullptr)
      {
        workerThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, 0, GetCurrentThreadId());
        CheckForError();
      }

      if (mainThread)
      {
        SuspendThread(mainThread);
        CheckForError();

        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL;

        GetThreadContext(mainThread, &context);
        CheckForError();

        ResumeThread(mainThread);
        CheckForError();

        samples.push_back(context);
      }
    }
  }

  typedef std::pair<string, DWORD64> SamplePair;
  bool StatCompare(SamplePair* left, SamplePair* right)
  {
    return left->second > right->second;
  }

  void SamplingProfiler::CheckForError() const
  {
    DWORD returnCode = GetLastError();
    if (returnCode != ERROR_SUCCESS)
    {
      __debugbreak();
    }
  }

  #pragma comment(lib, "Shlwapi.lib")
  #include "Shlwapi.h" //used for PathAppend and PathFileExists for file/directory management
  #include "Profiler\StackTrace.h"
  using std::ofstream;
  void SamplingProfiler::ExportResults()
  {
#pragma region CollateSamplesTaken
    float totalSamples = samples.size();
    Framework::InitSym();

    //Used for getting context information
    BOOL result;
    STACKFRAME64 stack_frame;
    struct {
      SYMBOL_INFO symbol_info;
      char buffer[MAX_PATH];
    } symbol;
    DWORD64 symbol_offset;

    //Used for adding to stats
    bool located;
    vector<SamplePair*> stats;
    CONTEXT* context;
    for (unsigned i = 0; i < samples.size(); ++i)
    {
      context = &samples[i];

      // Fill the initial stack frame information
      stack_frame = STACKFRAME64();
      Framework::FillStackFrame(stack_frame, *context);

      // Traverse the stack
      while (true)
      {
#pragma region ResolveSymbolInfo
        // Find the caller's parent
        result = StackWalk64(
          IMAGE_FILE_MACHINE_CURRENT, // IMAGE_FILE_MACHINE_I386 or IMAGE_FILE_MACHINE_AMD64
          GetCurrentProcess(), // Process
          GetCurrentThread(), // Thread
          &stack_frame, // Stack Frame Information
          &context, // Thread Context Information
          NULL, // Read Memory Call Back (Not Used)
          SymFunctionTableAccess64, // Function Table Accessor
          SymGetModuleBase64, // Module Base Accessor
          NULL); // Address Translator (Not Used)

        // Finished or untable to trace further
        if (result == FALSE) { break; }

        // Find the symbol name
        // This weird structure allows the SYMBOL_INFO to store a string of a user
        // defined maximum length without requiring any allocations or additional pointers
        symbol.symbol_info = SYMBOL_INFO();
        symbol.symbol_info.SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol.symbol_info.MaxNameLen = MAX_PATH;
        symbol_offset = 0;
        result = SymFromAddr(GetCurrentProcess(), stack_frame.AddrPC.Offset, &symbol_offset, &symbol.symbol_info);
#pragma endregion

#pragma region TallySymbols
        // If an associated symbol was found, print its name
        if (result && symbol.symbol_info.NameLen > 1 && symbol.symbol_info.NameLen < 200)
        {
          //TODO::Search for symbol name in stats and increment counter if it exists
          located = false;
          for (unsigned i = 0; i < stats.size() && !located; ++i)
          {
            if (strcmp(stats[i]->first.c_str(), symbol.symbol_info.Name) == 0)
            {
              stats[i]->second++;
              located = true;
            }
          }

          if (!located)
          {
            stats.push_back(new SamplePair(symbol.symbol_info.Name, 0));
          }
        }
#pragma endregion
      }
    }

    //Sort vector in descending frequency
    std::sort(stats.begin(), stats.end(), StatCompare);
#pragma endregion

    //Looks for existing folder "ProfilingReports" and if it doesn't exist creates one.
#pragma region ChangeDirToProfilingReports
    const int bufferSize = MAX_PATH;
    char oldDir[bufferSize]; // store the current directory

    // get the current directory, and store it
    if (!GetCurrentDirectory(bufferSize, oldDir)) {
      std::cerr << "Error getting current directory: #" << GetLastError();
    }
    std::cout << "Current directory: " << oldDir << '\n';

    // new directory
    char newDir[bufferSize];
    strcpy(newDir, oldDir);
    char* lpNewDir = newDir;
    char* lpAppendStr = R"(..\logs)";

    // old way of setting the directory to the profiling reports folder
    //  sprintf_s(newDir, "%s/%s", oldDir, R"(\..\ProfilingReports)");
    if (PathAppend(lpNewDir, lpAppendStr))
    {
      if (!PathFileExists(lpNewDir))
      {
        CreateDirectory(lpNewDir, NULL);
      }

      lpAppendStr = R"(\ProfilingReports)";
      if (PathAppend(lpNewDir, lpAppendStr))
      {
        if (!PathFileExists(lpNewDir))
        {
          CreateDirectory(lpNewDir, NULL);
        }

        if (!SetCurrentDirectory(lpNewDir))
        {
          std::cerr << "Error setting current directory: #" << GetLastError();
        }
        else
        {
          std::cout << "Set current directory to " << newDir << '\n';
        }
      }
    }
#pragma endregion 

    string filename = "profilingreport";
    filename.append(std::to_string(id));
    filename.append(".txt");

    std::cout << "Generating file " << filename << "...";
    ofstream myfile(filename.c_str());

    //uncomment std::cout << strBuffer to have profiler print to console window;
    char strBuffer[1024];
    sprintf_s(strBuffer, "Profile Date Info: %s\n", __TIMESTAMP__);
    myfile << std::endl << strBuffer;
    sprintf_s(strBuffer, "Total # of Samples: %u\n", (unsigned long long)totalSamples);
    myfile << std::endl << strBuffer;
    //std::cout << strBuffer;
    for (auto stat : stats)
    {
      sprintf_s(strBuffer, "Function: %s, Percentage: %f\n", stat->first.c_str(), 100 * (stat->second / totalSamples));
      myfile << strBuffer;
    //  std::cout << strBuffer;
      delete stat;
      stat = nullptr;
    }
    myfile.close();

    std::cout << " complete!" << std::endl;
    stats.clear();
    samples.clear();

    // Reset the current directory back to what it was.
#pragma region ResetCurrentDirectory
    if (!SetCurrentDirectory(oldDir)) 
    {
      std::cerr << "Error resetting current directory: #" << GetLastError();
    }
    else
    {
      std::cout << "Reset current directory. \n";
    }
#pragma endregion
  }
#endif
}
