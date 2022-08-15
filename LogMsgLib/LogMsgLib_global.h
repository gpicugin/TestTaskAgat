#ifndef LOGMSGLIB_GLOBAL_H
#define LOGMSGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGMSGLIB_LIBRARY)
#  define LOGMSGLIB_EXPORT Q_DECL_EXPORT
#else
#  define LOGMSGLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGMSGLIB_GLOBAL_H
