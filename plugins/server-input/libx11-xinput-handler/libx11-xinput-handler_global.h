#ifndef LIBX11XINPUTHANDLER_GLOBAL_H
#define LIBX11XINPUTHANDLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBX11XINPUTHANDLER_LIBRARY)
#  define LIBX11XINPUTHANDLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBX11XINPUTHANDLERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBX11XINPUTHANDLER_GLOBAL_H
