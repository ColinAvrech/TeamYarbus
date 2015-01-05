#pragma once
#include <windows.h>
#include <cstdio>
#include <dbghelp.h>
#pragma comment( lib, "dbghelp" )

namespace Framework
{
  #if defined(_M_AMD64)
    // x64
  #define GET_CONTEXT(c) \
  do {
    \
      RtlCaptureContext(&c); \
  } while (0)

  void FillStackFrame(STACKFRAME64& stack_frame, const CONTEXT& context) {
      stack_frame.AddrPC.Mode = AddrModeFlat;
      stack_frame.AddrPC.Offset = context.Rip;
      stack_frame.AddrStack.Mode = AddrModeFlat;
      stack_frame.AddrStack.Offset = context.Rsp;
      stack_frame.AddrFrame.Mode = AddrModeFlat;
      stack_frame.AddrFrame.Offset = context.Rbp;
    }

  #define IMAGE_FILE_MACHINE_CURRENT IMAGE_FILE_MACHINE_AMD64
    // x64
  #elif defined(_M_IX86)
    // This only works for x86
    // For x64, use RtlCaptureContext()
    // See: http://jpassing.wordpress.com/2008/03/12/walking-the-stack-of-the-current-thread/
    __declspec(naked) DWORD _stdcall GetEIP() {
      _asm {
        mov eax, dword ptr[esp]
          ret
      };
    }

    __declspec(naked) DWORD _stdcall GetESP() {
      _asm {
        mov eax, esp
          ret
      };
    }

    __declspec(naked) DWORD _stdcall GetEBP() {
      _asm {
        mov eax, ebp
          ret
      };
    }

    // Capture the context at the current location for the current thread
    // This is a macro because we want the CURRENT function - not a sub-function
  #define GET_CONTEXT(c) \
  do {\
  ZeroMemory(&c, sizeof(c)); \
  c.ContextFlags = CONTEXT_CONTROL; \
  c.Eip = GetEIP(); \
  c.Esp = GetESP(); \
  c.Ebp = GetEBP(); \
  } while (0)

    void FillStackFrame(STACKFRAME64& stack_frame, const CONTEXT& context) {
      stack_frame.AddrPC.Mode = AddrModeFlat;
      stack_frame.AddrPC.Offset = context.Eip;
      stack_frame.AddrStack.Mode = AddrModeFlat;
      stack_frame.AddrStack.Offset = context.Esp;
      stack_frame.AddrFrame.Mode = AddrModeFlat;
      stack_frame.AddrFrame.Offset = context.Ebp;
    }

  #define IMAGE_FILE_MACHINE_CURRENT IMAGE_FILE_MACHINE_I386
    // x86
  #else
  #error Unsupported Architecture
  #endif

  // Initialize the debug help libarary
  static void InitSym() 
  {
    // Only initialize once
    static bool ImghelpInitilized = false;
    if (ImghelpInitilized) { return; }

    // Initilize IMAGEHLP.DLL
    SymInitialize(GetCurrentProcess(), NULL, true);
    // OPTIONAL: Add support for line # in stack trace using SymGetLineFromAddr()
    //SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);

    ImghelpInitilized = true;
  }

  // Print the call stack entry for the given address
  void PrintCallStackEntry(DWORD64 addr) 
  {
    // Initialize the symbol information if not done already
    InitSym();

    // Find the module name
    IMAGEHLP_MODULE64 module = { 0 };
    module.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
    BOOL result = SymGetModuleInfo64(GetCurrentProcess(), addr, &module);

    // If an associated module was found, print its name
    printf("%s!0x%08x",
      (result) ? module.ModuleName : "",
      addr);

    // Find the symbol name
    // This weird structure allows the SYMBOL_INFO to store a string of a user
    // defined maximum length without requiring any allocations or additional pointers
    struct {
      SYMBOL_INFO symbol_info;
      char buffer[MAX_PATH];
    } symbol = { 0 };
    symbol.symbol_info.SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol.symbol_info.MaxNameLen = MAX_PATH;
    DWORD64 symbol_offset = 0;
    result = SymFromAddr(GetCurrentProcess(), addr, &symbol_offset, &symbol.symbol_info);

    // If an associated symbol was found, print its name
    if (result) {
      printf(" %s (0x%08x+0x%x)",
        symbol.symbol_info.Name,
        (DWORD)symbol.symbol_info.Address,
        (DWORD)symbol_offset);
    }

    printf("\r\n");
  }

  void StackTrace(const CONTEXT* start_context) 
  {
    InitSym();

    // Fill the initial context information
    CONTEXT context = *start_context;

    // Fill the initial stack frame information
    STACKFRAME64 stack_frame = { 0 };
    FillStackFrame(stack_frame, context);

    // Traverse the stack
    while (true)
    {
      // Find the caller's parent
      BOOL result = StackWalk64(
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

      // Print the call stack entry
      PrintCallStackEntry(stack_frame.AddrPC.Offset);
    }
  }
}
