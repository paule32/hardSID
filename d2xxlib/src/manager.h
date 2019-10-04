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
#ifndef MANAGER_H
#define MANAGER_H

#include "d2xxdevice.h"
#include "hardsidio.h"

HARDSID_NAMESPACE_BEGIN
class Manager:
        public ManagerNotifier,
        public Observer
{
private:
    DWORD dev_count;
    FT_STATUS ft_status;
    D2XXDevicesList dev_list;

    bool IsValidDeiviceInfo(FT_DEVICE_LIST_INFO_NODE *dev_info);
    DWORD CreateDeviceList(D2XXDevicesList *list);
    void CleanList(D2XXDevicesList *list);
public:
    explicit Manager();
    virtual ~Manager();

    DWORD Count(void);
    DWORD Rescan(void);
    D2XXDevice *GetDevice(DWORD index);
    DWORD FT_Status(void);
    void DisplayDevicesInfo(void);
    virtual void Update(Subject *subject);
};

HARDSID_NAMESPACE_END
#endif // MANAGER_H
