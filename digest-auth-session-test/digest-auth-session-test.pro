#-------------------------------------------------
#
# Project created by QtCreator 2015-09-03T00:13:52
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = digest-auth-session-test
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = release
TEMPLATE = app
OBJECTS_DIR=bin

SOURCES += main.cpp \
    ClientSocketHandler.cpp \
    SslHandler.cpp  \
    utils/fileutils.cpp \
    utils/stringutil.cpp \
    HashDigestListener.cpp

HEADERS += \
    SslHandler.h \
    ClientSocketHandler.h  \
    utils/fileutils.h \
    utils/stringutil.h \
    HashDigestListener.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpServer
else:unix: LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpServer

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpdecoder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpdecoder
else:unix: LIBS += -L$$PWD/../digest-auth-session-lib/libs/ -lhttpdecoder

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/release/ -ldigestauthsession
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../digest-auth-session-lib/release/ -ldigestauthsession
else:unix: LIBS += -L$$PWD/../digest-auth-session-lib/release/ -ldigestauthsession

INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/../digest-auth-session-lib/libs
INCLUDEPATH += $$PWD/../digest-auth-session-lib
DEPENDPATH += $$PWD/libs

QMAKE_CLEAN += -r $${PWD}/$${DESTDIR}

QMAKE_POST_LINK +=$$quote(rsync -avm --include=*/ --include=*.h --exclude=* $${PWD}/$${SOURCES_DIR}/ $${PWD}/$${DESTDIR})
