// ----------------------------------------------------------------------------
// Copyright (c) 2019 Jens Kallup - non-profit => kallup.jens@web.de
// Copyright (c) 2019 Andreas Schumm - gh0st
// Copyright (c)      Stein Pedersen - stein.pedersen@hue.no
//
// all rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "CommandReceiver.h"
#include "SIDBlasterInterfaceImpl.h"

#include <cassert>

namespace SIDBlaster {

CommandReceiver::CommandReceiver() : m_AdaptiveWriteBuffer(false) {
  for (int i = 0; i < TIMING_SAMPLES; ++i) {
    m_TimingSamples[i] = 0;
  }
  m_TimingSampleIndex = 0;
}

CommandReceiver::~CommandReceiver() {
  m_WaitEvent.DeAllocate();
}

void CommandReceiver::Initialize() {
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frq);
    QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTick);
    m_NextTick = m_StartTick;
    m_CycleScale = m_Frq / PAL_CLOCK;
    m_InvertedCycleScale = PAL_CLOCK / m_Frq;
    m_AccumulatedCycles = 0;
    m_WaitEvent.Allocate("Wait...");
    m_IsReadResultReady = false;
    m_ReadResult = 0;
    for (int i = 0; i < SIDBlasterEnumerator::Instance()->DeviceCount(); ++i) {
        m_Devices.push_back(SIDBlasterEnumerator::Instance()->CreateInterface(this, i));
    }
}

void
CommandReceiver::Uninitialize() {
  for (int i = 0; i < (int)m_Devices.size(); ++i) {
    SIDBlasterEnumerator::Instance()->ReleaseInterface(m_Devices[i]);
  }
  m_Devices.clear();
}

void CommandReceiver::ResetTimer() {
  QueryPerformanceCounter((LARGE_INTEGER*)&m_NextTick);
  m_StartTick = m_NextTick;
  m_AccumulatedCycles = 0;
}

int CommandReceiver::WaitForCycle(int cycle) {
  int64 tick = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&tick);
  int64 waitForTick = m_NextTick + (int64)(cycle * m_CycleScale);
  int64 waitDelta = waitForTick - tick;
  int msec = (int)(waitDelta * 1000 / m_Frq) - 1;
  if (msec > 0) {
    for (int i = 0; i < (int)m_Devices.size(); ++i) {
      m_Devices[i]->SoftFlush();
    }
    m_WaitEvent.Wait(msec);
  }
  while (tick < waitForTick) {
    QueryPerformanceCounter((LARGE_INTEGER*)&tick);
  }
  m_CurrentTick = tick;
  m_NextTick = waitForTick;
  m_AccumulatedCycles += cycle; // For debugging only
  return (int)(waitDelta * m_InvertedCycleScale);
}

void CommandReceiver::Log(const char* format, ...)
{
    va_list arg;
    char buffer[1024];
    va_start(arg, format);
    vsprintf(buffer, format, arg);
    va_end(arg);

    fprintf(stdout, "%s", buffer);
}

void CommandReceiver::LogTimestamp() {
    fprintf(stdout,"CYC: %08I64X ACC: %08I64X\n", CycleFromTick(m_CurrentTick), m_AccumulatedCycles);
}

uint64_t CommandReceiver::CycleFromTick(uint64_t tick) {
    return (uint64_t)((tick - m_StartTick) * m_InvertedCycleScale);
}

int
CommandReceiver::ExecuteCommand(CommandParams const& command) {
  if (m_Devices.empty()) {
    return 0;
  }
  bool buffer_writes = m_Devices[0]->WriteBufferSize() > 0 || m_AdaptiveWriteBuffer;
  int retval = 0;
  int waited_cycles = 0;
  if (command.m_Cycle > 0) {
    waited_cycles = WaitForCycle(command.m_Cycle);
  }
  m_TimingSamples[m_TimingSampleIndex] = waited_cycles;
  m_TimingSampleIndex = (m_TimingSampleIndex + 1) % TIMING_SAMPLES;
  //Log("Waited cycles: %d, buffer size: %d\n", waited_cycles, m_SIDBlaster->WriteBufferSize());
  if (waited_cycles < 0) {
    buffer_writes = true;
  }
  int sum = 0;
  if (m_AdaptiveWriteBuffer) {
    for (int i = 0; i < TIMING_SAMPLES; ++i) {
      sum += m_TimingSamples[i];
    }
    int avg = sum / TIMING_SAMPLES;
    if (avg < -10000) {
      for (int i = 0; i < (int)m_Devices.size(); ++i) {
        m_Devices[i]->SetWriteBufferSize(m_Devices[i]->WriteBufferSize() + 1);
      }
    }
  }
  //LogTimestamp();
  if (command.m_Device >= 0 && command.m_Device < (int)m_Devices.size()) {
    SIDBlasterInterface* sidblaster = m_Devices.at(command.m_Device);
    switch(command.m_Command) {
    case CommandParams::OpenDevice:
      sidblaster->Open();
      break;
    case CommandParams::CloseDevice:
      sidblaster->Close();
      break;
    case CommandParams::Delay:
      // Do nothing...
      break;
    case CommandParams::Write:
      if (buffer_writes) {
        sidblaster->BufferWrite(command.m_Reg, command.m_Data);
      } else {
        sidblaster->Write(command.m_Reg, command.m_Data);
      }
      break;
    case CommandParams::Read:
      {
        byte data = 0;
        retval = sidblaster->Read(command.m_Reg, data) | ((int)data << 8);
        if (data != 0) {
          int debug = 0;
        }
        m_ReadResult = retval;
        m_IsReadResultReady = true;
      }
      break;
    case CommandParams::Mute:
      sidblaster->Mute(command.m_Reg);
      break;
    case CommandParams::MuteAll:
      sidblaster->MuteAll();
      break;
    case CommandParams::Sync:
      sidblaster->Sync();
      ResetTimer();
      break;
    case CommandParams::SoftFlush: // What's the difference?
      sidblaster->SoftFlush();
      break;
    case CommandParams::Flush:
      sidblaster->Flush();
      ResetTimer();
      break;
    case CommandParams::Reset:
      sidblaster->Reset();
      ResetTimer();
      break;
    //case CommandParams::MuteLine: // What does this do?
    //  break;
    case CommandParams::Filter: // What does this do?
      break;
    case CommandParams::Lock: // Should ensure no other applications can obtain a sidblaster device
      break;
    case CommandParams::Unlock:
      break;
    default:
      ;
    }
  }
  return retval;
}

}
