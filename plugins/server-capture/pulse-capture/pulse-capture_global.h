#ifndef PULSECAPTURE_GLOBAL_H
#define PULSECAPTURE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PULSECAPTURE_LIBRARY)
#  define PULSECAPTURESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PULSECAPTURESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PULSECAPTURE_GLOBAL_H
