#include <Precompiled.h>
#include "Profiler\SamplingProfiler.h"

SamplingProfiler::SamplingProfiler(const unsigned& p_maxsamples) : maxSamples(p_maxsamples)
{
  mainThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, 0, GetCurrentThreadId());
  CheckForError();
  workerThread = nullptr;
  sampleEvent = timeSetEvent(1, 1, ProfilerCallback, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);
}

SamplingProfiler::~SamplingProfiler()
{
  if (!IsFull())
  {
    Exit();
  }
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

  ExportResults();
}

void SamplingProfiler::TakeSample()
{
  if (IsFull())
  {
    Exit();
  }
  else
  {
    if (workerThread == nullptr)
    {
      workerThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, 0, GetCurrentThreadId());
      CheckForError();
    }

    if (mainThread)
    {
      if (samples.size() < maxSamples)
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

#include "Profiler\StackTrace.h"
#include <algorithm>
#include <fstream>
using std::ofstream;
void SamplingProfiler::ExportResults()
{
  float totalSamples = (float)std::min(maxSamples, samples.size());
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

  string filename = "profilingreport";
  char strBuffer[1024];

#ifdef _DEBUG
  filename.append("_debug.txt");
#else
  filename.append("_release.txt");
#endif

  std::cout << "Generating file..." << std::endl;
  ofstream myfile(filename.c_str());
  sprintf_s(strBuffer, "Total # of Samples: %u\n", (unsigned long long)totalSamples);
  myfile << strBuffer;
  std::cout << strBuffer;
  for (auto stat : stats)
  {
    sprintf_s(strBuffer, "Function: %s, Percentage: %f\n", stat->first.c_str(), 100 *(stat->second / totalSamples));
    myfile << strBuffer;
    std::cout << strBuffer;
    delete stat;
    stat = nullptr;
  }
  myfile.close();

  std::cout << filename.c_str() << " complete!" << std::endl;
  stats.clear();
  samples.clear();
}

