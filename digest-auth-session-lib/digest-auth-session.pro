#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T13:52:17
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = digestauthsession
CONFIG+= staticlib

TEMPLATE = lib

OBJECTS_DIR=bin
DESTDIR = release
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    utils/stringutil.cpp \
    DigestManager.cpp \
    Digestinfo.cpp

HEADERS += \
    utils/stringutil.h \
    IDigestListener.h \
    digeststruct.h \
    DigestManager.h \
    Digestinfo.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lhttpServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lhttpServer
else:unix: LIBS += -L$$PWD/libs/ -lhttpServer

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lhttpdecoder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lhttpdecoder
else:unix: LIBS += -L$$PWD/libs/ -lhttpdecoder

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

QMAKE_CLEAN += -r $${PWD}/$${DESTDIR}

QMAKE_POST_LINK +=$$quote(rsync -avm --include=*/ --include=*.h --exclude=* $${PWD}/$${SOURCES_DIR}/ $${PWD}/$${DESTDIR})
