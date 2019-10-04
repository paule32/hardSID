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
#ifndef SIDBLASTER_SIDBLASTERAPI_H_INCLUDED
#define SIDBLASTER_SIDBLASTERAPI_H_INCLUDED

#include "SIDBlasterInterface.h"

namespace SIDBlaster {

struct CommandParams
{
    enum Command { // Keeps the order of command ids the same as the original windows host
        MinCommand = 0,
        Reset = MinCommand,
        Delay,
        Write,
        Read,
        Sync,
        Flush,
        Mute,
        MuteAll,
        SoftFlush,
        Lock,
        MuteLine,
        Filter = MuteLine,
        Unlock,
        OpenDevice,
        CloseDevice,
        NOP,
        MaxCommand
    }   m_Command;
    int m_Device;
    int m_Reg;
    int m_Data;
    int m_Cycle;
    int m_IsBuffered;

    CommandParams(int device = 0, Command cmd = NOP, int reg = 0, int data = 0, int cycle = -1, bool is_buffered = false) :
    m_Device(device),
    m_Command(cmd),
    m_Reg(reg),
    m_Data(data),
    m_Cycle(cycle),
    m_IsBuffered(is_buffered ? 1 : 0)
    {
    }
};

class CommandDispatcher {
public:
    virtual ~CommandDispatcher();
    virtual int   SendCommand(CommandParams const& cmd) = 0;
    virtual bool  IsAsync() = 0;
    virtual void  Initialize() = 0;
    virtual void  Uninitialize() = 0;
    virtual int   DeviceCount() = 0;
};

}
