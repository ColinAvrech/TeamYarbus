#pragma once
#include <Windows.h>
#include <vector>
using std::vector;

class SamplingProfiler
{
public:
  SamplingProfiler(const unsigned& p_maxsamples);
  ~SamplingProfiler();
  
  void Exit();

  bool IsFull(){ return samples.size() == maxSamples; }

  void TakeSample();

  void ExportResults();

  void CheckForError() const;

private:
  MMRESULT sampleEvent;
  const unsigned maxSamples;
  HANDLE mainThread, workerThread;
  vector<CONTEXT> samples;
};

void CALLBACK ProfilerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
