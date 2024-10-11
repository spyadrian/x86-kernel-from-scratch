#ifndef _TYPES_COMMON_H_
#define _TYPES_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

typedef unsigned char uchar_t;        //  8 bits
typedef unsigned short ushort_t;      // 16 bits
typedef unsigned int uint_t;          // 32 bits
typedef unsigned long long ulong_t;   // 64 bits

#ifndef NULL
#define NULL 0
#endif

#define UNUSED(x) ((void)(x))

#endif

