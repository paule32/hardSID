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
#ifndef COMMANDRECEIVER_H
#define COMMANDRECEIVER_H

#include <stdint.h>
#include <sys/types.h>

namespace SIDBlaster {
QVector<SIDBlasterInterface *> m_Devices;

class CommandReceiver
{
public:                   CommandReceiver();
                         ~CommandReceiver();
    int                   ExecuteCommand(CommandParams const& command);
    void                  Initialize();
    void                  Uninitialize();
    void                  ResetTimer();
    int                   WaitForCycle(int cycle);
    //virtual void          Log(const char* format, ...);
    //void                  LogTimestamp();

    inline int            DeviceCount() {
                            return (int)m_Devices.size();
                          }

    inline SIDBlasterInterface* Device(int deviceID) {
        if (deviceID >= 0 && deviceID < (int)m_Devices.size()) {
            return m_Devices.at(deviceID);
        } else {
            return NULL;
        }
    }

    inline bool           IsReadResultReady() {
                            return m_IsReadResultReady;
                          }
    inline int            ReadResult() {
                              m_IsReadResultReady = false;
                              return m_ReadResult;
                          }
    enum {
        TIMING_SAMPLES = 8
    };

    uint64_t              CycleFromTick(uint64_t tick);
    uint64_t              m_Frq; // Performance Frequency
    uint64_t              m_StartTick;
    uint64_t              m_NextTick;
    uint64_t              m_CurrentTick;
    double                m_CycleScale; // CPU_CYCLE * CYCLESCALE = TICKS (QPC ticks)
    double                m_InvertedCycleScale;

    uint64_t              m_AccumulatedCycles;

    bool                  m_AdaptiveWriteBuffer;

    EventObject           m_WaitEvent;

    int                   m_TimingSamples[TIMING_SAMPLES];
    int                   m_TimingSampleIndex;

    bool                  m_IsReadResultReady;
    int                   m_ReadResult;
};
}       // namespace
#endif  // COMMANDRECEIVER_H
