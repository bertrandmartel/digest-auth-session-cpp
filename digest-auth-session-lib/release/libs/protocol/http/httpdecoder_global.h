/**
    httpdecoder_global.h
    This will make access this library to QLibrary (no need to access header and lib)

    @author Bertrand Martel
    @version 1.0
*/
#ifndef HTTPDECODER_GLOBAL_H
#define HTTPDECODER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HTTPDECODER_LIBRARY)
#  define HTTPDECODERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HTTPDECODERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HTTPDECODER_GLOBAL_H
