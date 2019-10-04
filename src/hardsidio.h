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
#ifndef HARDSIDIO_H
#define HARDSIDIO_H

#include <QObject>
#include <QList>
#include <QVector>

#include "wintypes.h"
#include "../ftd2xx.h"

#include <assert.h>

HARDSID_NAMESPACE_BEGIN

enum DeviceEventType {
    E_UNKNOWN = 0,
    E_PLUGGED,
    E_UNPLUGGED
};

// ---------------------
// class forwarders ...
// ---------------------
class QObject;
class Subject;
class ManagerObserver;
class ManagerNotifier;

class        Observer ;
extern QList<Observer *> observers;

class HardSIDIO : public ::QObject
{
    Q_OBJECT
public:
    explicit HardSIDIO(QObject *parent = nullptr);
    virtual ~HardSIDIO() = 0;
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual DWORD Read(void *buffer, DWORD count) = 0;
    virtual DWORD Write(void *buffer, DWORD count) = 0;
};

class Observer: public ::QObject
{
    Q_OBJECT
public:
    explicit Observer(QObject *parent = nullptr);
    virtual ~Observer() = 0;
    virtual void Update(Subject *subject) = 0;
};

class Subject: public ::QObject
{
    Q_OBJECT
public:
    explicit Subject(QObject *parent = nullptr);
    virtual ~Subject() = 0;
    void Attach(Observer *observer) {
        Q_UNUSED(observer);
        observers.append(observer);
    }
    void Detach(Observer *observer) {
        QList<Observer *>::iterator it = observers.begin();
        while (it != observers.end()) {
            if (*it == observer) {
                observers.erase(it);
                break;
            }
        }
    }
    void Notify() {
        QList<Observer *>::iterator it = observers.begin();
        while (it != observers.end()) {
            (*it)->Update(this);
            ++it;
        }
    }
};

class ManagerObserver : public Observer
{
    Q_OBJECT
public:
    explicit ManagerObserver() {}
    virtual ~ManagerObserver() {}
    virtual void Update(Subject *_pSubject) { Q_UNUSED(_pSubject); }
    virtual void Update(ManagerNotifier *manager, void *device, DWORD event_code) = 0;
};

class ManagerNotifier : public Subject
{
    Q_OBJECT
public:
    explicit ManagerNotifier() {}
    virtual ~ManagerNotifier() = 0;
    void Notify(void *device, DWORD event_code)
    {
        assert(device);
        ::QList<Observer *>::iterator it = observers.begin();
        while (it != observers.end()) {
            ManagerObserver *obs = dynamic_cast<ManagerObserver *>(*it);
            if (obs) obs->Update(this, device, event_code);
            ++it;
        }
    }
};
HARDSID_NAMESPACE_END
#endif // HARDSIDIO_H
