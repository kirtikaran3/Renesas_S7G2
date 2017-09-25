/* Unix platform.h for EK */

#include <stdio.h>
#ifdef __GNUC__
#include <sys/stat.h>
#endif
#include "tx_api.h"

#ifndef IBUFLEN
#define IBUFLEN  4096			/* File input buffer size */
#endif /* IBUFLEN */

#ifndef OBUFLEN
#define OBUFLEN  8192                   /* File output buffer size */
#endif /* OBUFLEN */

#ifndef HAVE_UCHAR
#define HAVE_UCHAR
#endif

#ifndef HAVE_ULONG
#define HAVE_ULONG
#endif

#ifndef HAVE_USHORT
#define HAVE_USHORT
#endif

/* Do not perform a file scan */
#define NO_SCAN


