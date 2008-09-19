#ifndef __STDINTTYPES_H
#define __STDINTTYPES_H

#ifndef _MSC_VER
#include <stdint.h>
#else

/* FIXME (ivucica#7#) someone please fix 64-bit if needed!*/

typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#endif


#endif
