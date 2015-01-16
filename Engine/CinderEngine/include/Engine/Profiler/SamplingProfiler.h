#pragma once

namespace Framework
{
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

    void TakeSample(); //needs to be public for sampling callback

  private:
    void ExportResults();

    void CheckForError() const;

  private:
    int id;

    vector<CONTEXT> samples;
    unsigned maxSamples;
    bool full;

    //Data used for sample collection
    HANDLE mainThread = nullptr;
    HANDLE workerThread = nullptr;
    MMRESULT sampleEvent;
  };

  void CALLBACK ProfilerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
}
