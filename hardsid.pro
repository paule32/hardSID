# -----------------------------------------------------------------------------
# Copyright (c) 2019 Jens Kallup    - non-profit => kallup.jens@web.de
# Copyright (c) 2019 Andreas Schumm - gh0st
# Copyright (c)      Stein Pedersen - stein.pedersen@hue.no
#
# all rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
# THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# -----------------------------------------------------------------------------
TARGET   = hardsid
TEMPLATE = lib

QT      += core gui widgets xml svg network

DEFINES += QT_DEPRECATED_WARNINGS _GLIBCXX_USE_CXX11_ABI=0 DONT_DEFINE_QOBJECT
CONFIG  += release c++14

MYHOME   = /usr/local/Qt-5.8.0

TRANSLATIONS = hardsid_de.ts

QMAKE_CXXFLAGS += \
        -I. -I./ -O2 \
        -pthread \
        -I/usr/include/freetype2 \
        -I/usr/include/glib-2.0 \
        -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
        -I/usr/include/pixman-1 \
        -I/usr/include/libpng12 \
        \
        -I/usr/local/Qt-5.8.0/include/QtCore \
        -I/usr/local/Qt-5.8.0/include/QtSvg \
        -I/usr/local/Qt-5.8.0/include/QtNetwork \
        -I/usr/local/Qt-5.8.0/include/QtWidgets \
        -I/usr/local/Qt-5.8.0/include/QtX11Extras \
        -I/usr/local/Qt-5.8.0/include/QtXml \
        -I/usr/local/Qt-5.8.0/include/QtXmlPatterns \
        \
        -I${MYHOME}/include \
        -I${MYHOME}/include/QtCore \
        -I${MYHOME}/include/QtSvg \
        -I${MYHOME}/include/QtNetwork \
        -I${MYHOME}/include/QtWidgets \
        -I${MYHOME}/include/QtX11Extras \
        -I${MYHOME}/include/QtXml \
        -I${MYHOME}/include/QtXmlPatterns

SOURCES += \
        hardsidio.cpp \
    d2xxdevice.cpp \
    manager.cpp \
    main.cpp

HEADERS += \
        hardsidio.h \
        wintypes.h \
    d2xxdevice.h \
    ftd2xx.h \
    manager.h

FORMS +=

RESOURCES +=

DISTFILES += \
        hardsid_de.ts

LIBS += -L. -lvlc -lSDL
LIBS += -lfontconfig -lfreetype
LIbS += -lpthread -lgthread-2.0 -lglib-2.0

