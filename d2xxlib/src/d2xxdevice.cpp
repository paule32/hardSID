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
#include "d2xxdevice.h"
#include <QDebug>

HARDSID_NAMESPACE_BEGIN

# define FT_OPEN_BY_INDEX  8

# define FT_READ_TIMEOUT   1000
# define FT_WRITE_TIMEOUT  1000
# define FT_BAUD_RATE      500000 //FT_BAUD_115200
# define FT_WORD_LENGTH    FT_BITS_8
# define FT_STOP_BITS      FT_STOP_BITS_1
# define FT_PARITY         FT_PARITY_NONE

// -------------------------
// forward declarations ...
// -------------------------
class D2xxManager;
class D2xxManager *GetD2XXManager(void);

D2xxDevice::D2xxDevice(const FT_DEVICE_LIST_INFO_NODE *device_info)
{
    assert(device_info);
    memcpy(&info, device_info, sizeof(FT_DEVICE_LIST_INFO_NODE));
}
D2xxDevice::~D2xxDevice() {
    Close();
}

bool D2xxDevice::OpenEx(void* arg, DWORD flag)
{
    Close();

    if (flag & FT_OPEN_BY_INDEX)
    ft_status = FT_Open(  static_cast<int>(reinterpret_cast<intptr_t>(arg)), &handle);  else
    ft_status = FT_OpenEx(reinterpret_cast<void*>(arg), flag, &handle);

    if (FT_SUCCESS(ft_status))
    info.Flags |= FT_FLAGS_OPENED;

    return FT_SUCCESS(ft_status);
}

bool D2xxDevice::OpenByIndex(DWORD index) {
    return OpenEx((void*)index, FT_OPEN_BY_INDEX);
}

bool D2xxDevice::OpenBySerialNumber(const char *serial_number) {
    return OpenEx((PVOID)serial_number, FT_OPEN_BY_SERIAL_NUMBER);
}

bool D2xxDevice::OpenByDescription(const char *description) {
    return OpenEx((PVOID)description, FT_OPEN_BY_DESCRIPTION);
}

bool D2xxDevice::OpenByLocation(DWORD location) {
    return OpenEx((void*)location, FT_OPEN_BY_LOCATION);
}


bool D2xxDevice::Open() {
    return OpenEx(info.SerialNumber, FT_OPEN_BY_SERIAL_NUMBER);
}

void D2xxDevice::Close(void)
{
    if (handle) {
        if (FT_SUCCESS(ft_status = FT_Close(handle)))
        info.Flags &= ~FT_FLAGS_OPENED;

        handle = NULL;
        ft_status = FT_DEVICE_NOT_OPENED;
    }
}

DWORD D2xxDevice::Read(void *buffer, DWORD count)
{
    DWORD rw = 0;
    qDebug() << QString("\tReading %1 bytes: ").arg((qlonglong)count);

    if (FT_SUCCESS(ft_status = FT_Read(handle, buffer, count, &rw))) {
        qDebug() << QString(" (%1)\n").arg((long int*)rw);
        return rw;
    }
    else {
        printf(" ERROR\n");
        return 0;
    }   return 0;
}

DWORD D2xxDevice::Write(void *buffer, DWORD count) {
    DWORD rw = 0;
    qDebug() << QString("\tWriting %1 bytes: ").arg(count);
    DWORD ft_status = FT_Write(handle, buffer, count, &rw);

    if (FT_SUCCESS(ft_status)) {
        qDebug() << QString(" (%1)\n").arg(rw);
        return rw;
    }
    else {
        printf(" ERROR\n");
        return 0;
    }   return 0;
}

bool D2xxDevice::IsOpen() {
    return (handle != NULL);
}

bool D2xxDevice::Initialise(void) {
    bool ret = true;
    ret &= SetTimeouts(FT_READ_TIMEOUT, FT_WRITE_TIMEOUT);
    ret &= SetBaudRate(FT_BAUD_RATE);
    ret &= SetDataCharacteristics(FT_WORD_LENGTH, FT_STOP_BITS_1, FT_PARITY_NONE);
    return ret;
}

bool D2xxDevice::SetTimeouts(DWORD read_timeout, DWORD write_timeout) {
    return (FT_SUCCESS(ft_status = FT_SetTimeouts(handle, read_timeout, write_timeout)));
}

bool D2xxDevice::SetBaudRate(DWORD baud_rate) {
    return (FT_SUCCESS(ft_status = FT_SetBaudRate(handle, baud_rate)));
}

bool D2xxDevice::SetDataCharacteristics(BYTE word_length, BYTE stop_bits, BYTE parity) {
    return (FT_SUCCESS(ft_status = FT_SetDataCharacteristics(handle, word_length, stop_bits, parity)));
}

bool D2xxDevice::GetQueueStatus(DWORD *rx_bytes, DWORD *tx_bytes, DWORD *event_status) {
    return FT_SUCCESS(ft_status = FT_GetStatus(handle, rx_bytes, tx_bytes, event_status));
}

#ifdef USE_FTCHIPID
BOOL D2XXDevice::GetUniqueChipID(DWORD *chip_id) {
  return FT_SUCCESS(ft_status = FTID_GetChipIDFromHandle(handle, chip_id));
}
#endif

DWORD D2xxDevice::FT_Status(void) {
  return ft_status;
}

DWORD D2xxDevice::GetType(void) {
  return info.Type;
}

DWORD D2xxDevice::GetVIDPID(void) {
  return info.ID;
}

DWORD D2xxDevice::GetLocationID(void) {
  return info.LocId;
}

const char *D2xxDevice::GetSerialNumber(void) {
  return info.SerialNumber;
}

const char *D2xxDevice::GetDescription(void) {
  return info.Description;
}

void D2xxDevice::DisplayInfo(void)
{
  char *dev_type_str[] = {"232BM", "232AM", "100AX", "UNKNOWN", "2232C", "232R", "2232H", "4232H", "232H"};

  fprintf(stdout,"%18s%s\n", "FT Device type: ", dev_type_str[info.Type]);
  fprintf(stdout,"%18s%s\n", "Serial number: ", info.SerialNumber);
  fprintf(stdout,"%18s%s\n", "Description: ", info.Description);
  fprintf(stdout,"%18s0x%08X\n", "VID&PID: ", info.ID);
  fprintf(stdout,"%18s%d\n", "Is opened: ", (info.Flags & FT_FLAGS_OPENED));
  fprintf(stdout,"%18s0x%08X\n", "Handle: ",static_cast<int>(reinterpret_cast<intptr_t>((info.ftHandle))));
  fprintf(stdout,"%18s0x%08X\n", "Location ID: ", info.LocId);

  fflush(stdout);
}

DWORD D2xxDevice::Send(QVector<unsigned char> const& data) {
    if (data.size()) {
        return Write((void*)&data[0], (DWORD)data.size());
    }
    return 0;
}

DWORD D2xxDevice::Send(unsigned char const* data, size_t size) {
    if (size) {
        return Write((void*)data, (DWORD)size);
    }
    return 0;
}

DWORD D2xxDevice::Recv(QVector<unsigned char>& data) {
    DWORD
    rx_bytes = 0,
    tx_bytes = 0,
    event_status = 0;

    if (GetQueueStatus(&rx_bytes, &tx_bytes, &event_status) && rx_bytes) {
        data.resize(rx_bytes);
        return Read((void*)&data.at(0), rx_bytes);
    }
    return 0;
}
HARDSID_NAMESPACE_END
