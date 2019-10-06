
0240 7CDU EGGO VIJZ LEFF CI00 1829 843F
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
#ifndef D2XXDEVICE_H
#define D2XXDEVICE_H

#include "../ftd2xx.h"
#include "hardsidio.h"

HARDSID_NAMESPACE_BEGIN

class D2xxDevice : public HardSIDIO
{  
private:
    FT_HANDLE handle;
    FT_STATUS ft_status;
    FT_DEVICE_LIST_INFO_NODE info;

    bool OpenEx(void* arg, DWORD flag);
    bool OpenByIndex(DWORD index);
    bool OpenBySerialNumber(const char *serial_number);
    bool OpenByDescription(const char *description);
    bool OpenByLocation(DWORD location);

public:
    explicit D2xxDevice(const FT_DEVICE_LIST_INFO_NODE *device_info);
    virtual ~D2xxDevice();

    virtual bool Open();
    virtual void Close(void);

    DWORD Read (void *buffer, DWORD count);
    DWORD Write(void *buffer, DWORD count);

    bool IsOpen();
    bool Initialise(void);
    bool SetTimeouts(DWORD read_timeout, DWORD write_timeout);
    bool SetBaudRate(DWORD baud_rate);
    bool SetDataCharacteristics(BYTE word_length, BYTE stop_bits, BYTE parity);
    bool GetQueueStatus(DWORD *rx_bytes, DWORD *tx_bytes, DWORD *event_status);

#ifdef USE_FTCHIPID
    bool GetUniqueChipID(DWORD *chip_id);
#endif

    DWORD FT_Status(void);
    DWORD GetType(void);
    DWORD GetVIDPID(void);
    DWORD GetLocationID(void);

    const char * GetSerialNumber(void);
    const char * GetDescription(void);
    void DisplayInfo(void);

    DWORD Send(QVector<unsigned char> const& data);
    DWORD Send(unsigned char const* data, size_t size);
    DWORD Recv(QVector<unsigned char>& data);
};

typedef QVector<D2xxDevice *> D2xxDevicesList;
HARDSID_NAMESPACE_END

#endif // D2XXDEVICE_H
