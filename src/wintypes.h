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
#ifndef WINTYPES_H
#define WINTYPES_H

#include <stdint.h>
#include <sys/types.h>

// ----------------------------------------------------------------------------
// the following macro is only for the QtCreator CLANG Parser,
// to avoid many dirty screen output at right side of editor ...
//
// hack by: paule32
// ----------------------------------------------------------------------------
#ifndef DONT_DEFINE_QOBJECT
# define CODING_TIME
# define Q_OBJECT
#endif

#ifdef BOOL
#undef BOOL
typedef bool BOOL;
#endif

typedef unsigned char    UCHAR;
typedef unsigned short  USHORT;

typedef unsigned char * PUCHAR;
typedef unsigned long * PULONG;

typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef void      VOID;
typedef VOID *   PVOID;
typedef char *   PCHAR;

typedef DWORD HANDLE;

typedef long int * LPVOID;
typedef long int * LPWORD;
typedef long int * LPDWORD;

typedef uint32_t ULONG;
typedef ULONG  * ULONG_PTR;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        PVOID Pointer;
    } DUMMYUNIONNAME;
    HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;

#define HARDSID_NAMESPACE_BEGIN namespace D2xxLib {
#define HARDSID_NAMESPACE_END   }

#endif // WINTYPES_H
