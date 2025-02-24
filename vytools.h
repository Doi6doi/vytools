#ifndef VYTOOLSH
#define VYTOOLSH

/**
# vytools.h

[VyTools](index) is a *C* and *C++* library which
makes often used tasks easier.

The `vytools.h` header is for the *C* part.
It contains some fixed-size basic types,
often used structures, basic string and stream handling
and a resizable buffer. */

/** ## Contents
\toc */

/** ## Details */

/** macros to write extern "C" block
\ref #define VYT_CBEGIN()
\ref #define VYT_CEND() */
#ifdef __cplusplus
#define VYT_CBEGIN() extern "C" {
#define VYT_CEND() }
#else
#define VYT_CBEGIN()
#define VYT_CEND()
#endif

VYT_CBEGIN()

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "vytools_arch.h"

/// Typeless pointer
typedef void *  VytPtr;
/// C string
typedef const char * VytStr;
/// Unsigned 32-bit integer
typedef uint32_t VytU;
/// Signed 32-bit integer
typedef int32_t VytI;
/// Unsigned 64-bit integer
typedef uint64_t VytZ;
/// 32-bit floating point value
typedef float VytF;

/** 2 coordinate vector
\ref struct Vyt?Vec2 { */
#define VYT_VEC2( t ) \
typedef struct Vyt_##t##Vec2 { \
   Vyt##t x; /** X coordinate */ \
   Vyt##t y; /** Y coordinate */ \
} * Vyt##t##Vec2;

VYT_VEC2( U )

/** Rectangle
\ref struct Vyt?Rect { */
#define VYT_RECT( t ) \
typedef struct Vyt_##t##Rect { \
   Vyt##t left; /** Left X coordinate */ \
   Vyt##t top; /** Top Y coordinate */ \
   Vyt##t width; /** Width */ \
   Vyt##t height; /** Height */ \
} * Vyt##t##Rect;

VYT_RECT( U )

/** 2d transformation matrix
\ref struct Vyt?Trans2 { */
#define VYT_TRANS2( t ) \
typedef struct Vyt_##t##Trans2 { \
   Vyt##t sx; /** X coordinate scale */ \
   Vyt##t rx; /** X coordinate rotate or skew */ \
   Vyt##t ry; /** Y coordinate rotate or skew */ \
   Vyt##t sy; /** Y coordinate scale */ \
   Vyt##t dx; /** X coordinate shift */ \
   Vyt##t dy; /** Y coordinate shift */ \
} * Vyt##t##Trans2;

VYT_TRANS2( F )

// point cloud
typedef struct Vyt_Cloud {
   VytU weight; // number of points 
   float mx; // weight point x coordinate
   float my; // weight point y coordinate
   float dx; // avergae direction x coordinate
   float dy; // avergae direction y coordinate
} * VtlCloud;

/// Any stream type
typedef VytPtr VytStream;

/// Resizable allocated memory
typedef struct Vyt_Mem * VtlMem;

/** Custom read or write operation
\param stream Pointer to the stream
\param mem Pointer to data
\param size Number of bytes to read or write
\return Number of bytes successfully read or written */
typedef VytU (* VytStreamOp)( VytStream stream, VytPtr mem, VytU size );

/// `[fread](https://en.cppreference.com/w/c/io/fread)` as `VytStreamOp`
VytU vyt_fread( VytStream stream, void VytPtr mem, VytU size );
/// `[fread](https://en.cppreference.com/w/c/io/fread)` as `VytStreamOp`, can read less than full block
VytU vyt_fread_part( VytStream stream, VytPtr mem, VytU size );
/// `[fwrite](https://en.cppreference.com/w/c/io/fwrite)` as `VytStreamOp`
VytU vyt_fwrite( VytStream stream, VytPtr mem, VytU size );

/** write or read a complete block using `VytStreamOp`.
Keeps calling `op` until `size` is reached or `0` is returned
\param stream Pointer to the stream
\param op The read or write operation
\param mem Pointer to data
\param size Total number of bytes to write
\return `true` if all `size` bytes are read or written */
bool vyt_block_op( VytStream stream, VytStreamOp op, VytPtr mem, VytU size );

/** skip reading bytes.
Calls `read` without using the data read
\param stream Pointer to the stream
\param read The read or write operation
\param size Total number of bytes to skip
\return `true` if all `size` bytes are skipped */
bool vyt_read_skip( VytStream stream, VytStreamOp read, VytU size );

/** write line to std error.
\param ... Arguments as in [printf](https://en.cppreference.com/w/c/io/fprintf)*/
#define vyt_ewrite( ... ) { \
   fprintf( stderr, __VA_ARGS__ ); \
   fprintf( stderr, "\n" ); \
   fflush( stderr ); \
}

/** write line to std error and exit program.
\param ... Arguments as in [printf](https://en.cppreference.com/w/c/io/fprintf)*/
#define vyt_die( ... ) { \
   vyt_ewrite( __VA_ARGS__ ); \
   exit(1); \
}

/** converts string to natural number
\param s The string
\param nat Number is returned here
\return `true` on success */
bool vyt_nat( VytStr s, VytU * nat );

/** same strings (`NULL` proof)
\param a First string
\param b Second string
\return `true` if both are `NULL` or the two are the same */
bool vyt_same( VytStr a, VytStr b );

/** format string (uses static buffer)
\param fmt Format string
\param ... `[printf](https://en.cppreference.com/w/c/io/fprintf)` arguments
\return The formatted string */
VytStr vyt_sprintf( VytStr fmt, ... );

/// milliseconds passed since epoch
VytZ vyt_stamp();

/// milliseconds passed since last call to `vyt_stamp_diff`
VytU vyt_stamp_diff();

/** allocate resizable memory
\param size Memory size in bytes
\return Handle to memory */
VtlMem vyt_mem_create( VytU size );

/** free resizable memory
\param m Handle to memory */
void vyt_mem_free( VtlMem m );

/** resize resizable memory
\param m Handle to memory
\param size New size in bytes
\return `true` if succeeded */
bool vyt_mem_resize( VtlMem m, VytU size );

/** allocated resizable memory address
\param m Handle to memory
\return Pointer to allocated memory */
VytPtr vyt_mem_address( VtlMem m );

/** resizable memory current size
\param m Handle to memory
\return Current size of memory */
VytU vyt_mem_size( VtlMem m );

VYT_CEND()

#endif // VULTOOLSH
