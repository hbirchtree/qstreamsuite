#ifndef LIBAVCAPTURE_GLOBAL_H
#define LIBAVCAPTURE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBAVCAPTURE_LIBRARY)
#  define LIBAVCAPTURESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBAVCAPTURESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBAVCAPTURE_GLOBAL_H