#ifndef VYTOOLSARCHH
#define VYTOOLSARCHH

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>

#define VT_L16(x) x
#define VT_L32(x) x

#elif defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#define VT_L16(x) htole16(x)
#define VT_L32(x) htole32(x)

#endif

#endif // VYTOOLSARCHH
