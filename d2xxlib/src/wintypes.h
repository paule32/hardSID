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

//typedef signed char      int8_t;
//typedef signed      int  int16_t;
//typedef signed long int  int32_t;
//typedef signed long long int64_t;

//typedef unsigned char      uint8_t;
//typedef unsigned      int  uint16_t;
//typedef unsigned long int  uint32_t;
//typedef unsigned long long uint64_t;

#ifdef LINUX64
typedef unsigned long long ULONG_PTR;
typedef unsigned long long  UINT_PTR;
typedef unsigned int       UHALF_PTR;
typedef int                 HALF_PTR;
typedef          long long  LONG_PTR;

typedef unsigned long long ULONGLONG;
#else
typedef short           HALF_PTR;
typedef unsigned short UHALF_PTR;
typedef unsigned int    UINT_PTR;
typedef unsigned long  ULONG_PTR;
typedef          long   LONG_PTR;

typedef double ULONGLONG;
#endif

#define CONST const
#define VOID  void

typedef char  CHAR;
typedef char CCHAR;

typedef int            BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned short SHORT;

typedef unsigned int   DWORD;
typedef unsigned long  QWORD;

typedef unsigned char  DWORD8;
typedef unsigned short DWORD16;
typedef unsigned int   DWORD32;
typedef unsigned long  DWORD64;

typedef signed int   INT;
typedef signed char  INT8;
typedef signed short INT16;
typedef signed int   INT32;
typedef signed long  INT64;

typedef unsigned int   UINT;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned int   UINT32;
typedef unsigned long  UINT64;

typedef float  FLOAT;
typedef double DOUBLE;

typedef WORD LANGID;
typedef BYTE           BOOLEAN;
typedef      BOOLEAN *PBOOLEAN;

typedef BOOL *PBOOL;
typedef WORD *PWORD;

typedef void *PVOID;
typedef void *LPVOID;

typedef PVOID  HANDLE;
typedef HANDLE HACCEL;

typedef DWORD *LPDWORD;
typedef DWORD COLORREF;

typedef unsigned long int DWORDLONG;

typedef ULONG_PTR DWORD_PTR;

typedef UINT_PTR WPARAM;

typedef unsigned char      UCHAR;
typedef unsigned short     USHORT;
typedef unsigned long      ULONG;
typedef unsigned int       ULONG32;
typedef unsigned long long ULONG64;

typedef unsigned char TBYTE;
typedef char          TCHAR;

typedef ULONG_PTR SIZE_T;
typedef  LONG_PTR SSIZE_T;

typedef wchar_t WCHAR;

typedef WCHAR *LPWSTR;
typedef WORD  *LPWORD;
typedef WCHAR *PWCHAR;
typedef WCHAR *PWSTR;

typedef CHAR *LPSTR;
typedef LPSTR LPTSTR;

typedef CONST WCHAR *PCWSTR;
typedef CONST WCHAR *LPCWSTR;

typedef CONST void *LPCVOID;
typedef CONST CHAR *LPCSTR;

typedef LPCSTR LPCTSTR;

typedef long *LPLONG;
typedef int  *LPINT;

typedef CHAR *PSTR;
typedef CHAR *PCHAR;
typedef BYTE *PBYTE;

typedef TBYTE *PTBYTE;
typedef TCHAR *PTCHAR;

typedef USHORT  *PUSHORT;
typedef ULONG   *PULONG;
typedef ULONG32 *PULONG32;
typedef ULONG64 *PULONG64;

typedef UINT_PTR  *PUINT_PTR;

typedef ULONG_PTR *PULONG_PTR;
typedef ULONGLONG *PULONGLONG;

typedef UCHAR *PUCHAR;
typedef ULONG *PULONG;

typedef VOID *PVOID;
typedef char *PCHAR;

typedef unsigned long int ULONG;


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
