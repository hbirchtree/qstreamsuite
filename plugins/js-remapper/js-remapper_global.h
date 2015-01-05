#ifndef JSREMAPPER_GLOBAL_H
#define JSREMAPPER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JSREMAPPER_LIBRARY)
#  define JSREMAPPERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define JSREMAPPERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // JSREMAPPER_GLOBAL_H
