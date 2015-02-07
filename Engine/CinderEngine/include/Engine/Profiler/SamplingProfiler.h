#pragma once

namespace Framework
{
#ifdef _DEBUG
  #define NOMAX 0
  static unsigned ProfilesTaken = 0;

  class SamplingProfiler
  {
  public:
    
    SamplingProfiler(const unsigned& p_maxsamples);

    ~SamplingProfiler();

    int GetID(){ return id; }

    void Exit(); 

    bool IsFull(); //used to stop collecting samples on full profiles
    void SetFull();

    bool IsExported(){ return exported; }
    void SetExported();
    
    void TakeSample(); //needs to be public for sampling callback

  private:
    void ExportResults();

    void CheckForError() const;

  private:
    int id;

    vector<CONTEXT> samples;
    unsigned maxSamples;
    bool full;
    bool exported;
    bool exited;

    //Data used for sample collection
    HANDLE mainThread = nullptr;
    HANDLE workerThread = nullptr;
    MMRESULT sampleEvent;
  };

  void CALLBACK ProfilerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
#endif
}
