#pragma once

namespace Framework
{
  static unsigned ProfilingSamplesTaken = 0;

  class SamplingProfiler
  {
  public:
    SamplingProfiler(const unsigned& p_maxsamples);

    ~SamplingProfiler();

    void Exit();

    void TakeSample();

  private:
    bool IsFull();

    void ExportResults();

    void CheckForError() const;

  private:
    MMRESULT sampleEvent;
    unsigned maxSamples;
    HANDLE mainThread = nullptr;
    HANDLE workerThread = nullptr;
    vector<CONTEXT> samples;
  };

  void CALLBACK ProfilerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
}
