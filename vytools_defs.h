#ifndef VYTOOLSDEFSH
#define VYTOOLSDEFSH

/**
# vytools.h

[VyTools](index) is a *C* and *C++* library which
makes often used tasks easier.

The `vytools_defs.h` header is both for the *C* and *C++* parts.
It contains some preprocessor macros.
*/

/** ## Contents
\toc */

/** ## Details */

/** Export symbols in windows
\ref #define VYT_EXPORT */

/** Convert 16-bit integer to little-endian format
\ref #define VT_L16 */

/** Convert 32-bit integer to little-endian format
\ref #define VT_L32 */

/** Start of extern "C" block in C++
\ref #define VYT_CBEGIN() */

/** End of extern "C" block in C++
\ref #define VYT_CEND() */

#ifdef _WIN32

#ifdef VYT_LIB
#define VYT_EXPORT __declspec(dllexport)
#else
#define VYT_EXPORT __declspec(dllimport)
#endif

#include <winsock2.h>

#define VT_L16(x) x
#define VT_L32(x) x

#else

#define VYT_EXPORT

#if defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>

#define VT_L16(x) htole16(x)
#define VT_L32(x) htole32(x)

#endif

#endif

#ifdef __cplusplus
#define VYT_CBEGIN() extern "C" {
#define VYT_CEND() }
#else
#define VYT_CBEGIN()
#define VYT_CEND()
#endif

#endif // VYTOOLSDEFSH
