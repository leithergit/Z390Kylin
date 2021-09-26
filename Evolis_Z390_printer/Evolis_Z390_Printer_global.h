#ifndef EVOLIS_Z390_PRINTER_GLOBAL_H
#define EVOLIS_Z390_PRINTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EVOLIS_Z390_PRINTER_LIBRARY)
#  define EVOLIS_Z390_PRINTER_EXPORT Q_DECL_EXPORT
#else
#  define EVOLIS_Z390_PRINTER_EXPORT Q_DECL_IMPORT
#endif

#endif // EVOLIS_Z390_PRINTER_GLOBAL_H