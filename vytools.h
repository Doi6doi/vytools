#ifndef VYTOOLSH
#define VYTOOLSH

/**
[VyTools](index) is a *C* and *C++* library which
makes often used tasks easier.

The `vytools.h` header is for the *C* part.
It contains some fixed-size basic types,
often used structures, basic string and stream handling
and a resizable buffer.
*/

/** macros to write extern "C" block
\ref #define VYT_CBEGIN()
\ref #define VYT_CEND()
*/
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

/// C string
typedef const char * VytStr;
/// unsigned 32-bit integer
typedef uint32_t VytU;
/// signed 32-bit integer
typedef int32_t VytI;
/// 32-bit floating point value
typedef float VytF;
/// unsigned 64-bit integer
typedef uint64_t VytZ;

/** 2 coordinate vector
\ref struct Vyt[UILFGZ]Vec2 { 
*/
#define VYT_VEC2( t ) \
typedef struct Vyt_##t##Vec2 { \
   Vyt##t x; /** x coordinate */ \
   Vyt##t y; /** y coordinate */ \
} * Vyt##t##Vec2;

VYT_VEC2( U )

/** rectangle
\field left left coorinate of rect
\field top top coordinate of rect
\field width width of rect
\field height height of rect
*/
#define VYT_RECT( t ) \
typedef struct Vyt_##t##Rect { \
   Vyt##t left, top, width, height; \
} * Vyt##t##Rect;

VYT_RECT( U )

/** 2d transformation matrix
\field sx x coordinate sclae
\field rx x coordinate rotate or skew
\field sy y coordinate scale
\field ry y coordinate rotate or skew
\field dx x coordinate shift
\field dy y coordinate shift
*/
#define VYT_TRANS2( t ) \
typedef struct Vyt_##t##Trans2 { \
   Vyt##t sx, rx, ry, sy, dx, dy; \
} * Vyt##t##Trans2;

VYT_TRANS2( F )

/// point cloud
typedef struct Vyt_Cloud {
   /// number of points
   VytU weight;
   /// weight point
   float mx, my;
   /// avergae direction
   float dx, dy;
} * VtlCloud;

/// resizable allocated memory
typedef struct Vyt_Mem * VtlMem;

/// stream read or write error
#define VYT_STREAMERR -15001

/** custom read or write operation
\field stream pointer to the stream
\field mem pointer to data
\field size number of bytes to read or write
\return number of bytes successfully read or written
*/
typedef VytU (* VytStreamOp)( void * stream, void * mem, VytU size );

/// fread as `VytStreamOp`
VytU vyt_fread( void * stream, void * mem, VytU size );
/// fread as `VytStreamOp`, can read less than full block
VytU vyt_fread_part( void * stream, void * mem, VytU size );
/// fwrite as `VytStreamOp`
VytU vyt_fwrite( void * stream, void * mem, VytU size );

/** write or read a complete block using `VytStreamOp`
Keeps calling `op` until `size` is reached or `0` is returned
\field stream pointer to the stream
\field op the read or write operation
\field mem pointer to data
\field size total number of bytes to write
\return true if all `size` bytes are read or written
*/
bool vyt_block_op( void * stream, VytStreamOp op, void * mem, VytU size );

/** skip reading bytes
Calls `read` without using the data read
\field stream pointer to the stream
\field read the read or write operation
\field size total number of bytes to skip
\return true if all `size` bytes are skipped
*/
bool vyt_read_skip( void * stream, VytStreamOp read, VytU size );

/** write line to std error
All [printf](https://en.cppreference.com/w/c/io/fprintf) patterns can be used
*/
#define vyt_ewrite( ... ) { \
   fprintf( stderr, __VA_ARGS__ ); \
   fprintf( stderr, "\n" ); \
   fflush( stderr ); \
}

/** write line to std error and exit program
All [printf](https://en.cppreference.com/w/c/io/fprintf) patterns can be used
*/
#define vyt_die( ... ) { \
   vyt_ewrite( __VA_ARGS__ ); \
   exit(1); \
}

/** converts string to natural number
\field s the string
\fiels nat number is returned here
\return true on success
*/
bool vyt_nat( VytStr s, VytU * nat );

/** same strings (NULL proof)
\field a first string
\field b second string
\return true if both are `NULL` or the two are the same
*/
bool vyt_same( VytStr a, VytStr b );

/** format string (uses static buffer)
\field fmt format string
\field ... [printf](https://en.cppreference.com/w/c/io/fprintf) arguments
\return the formatted string
*/
VytStr vyt_sprintf( VytStr fmt, ... );

/// milliseconds passed since epoch
VytZ vyt_stamp();
/// milliseconds passed since last call to `vyt_stamp_diff`
VytU vyt_stamp_diff();

/** allocate resizable memory
\field size memory size in bytes
\return handle to memory
*/
VtlMem vyt_mem_create( VytU size );

/** free resizable memory
\field m handle to memory
*/
void vyt_mem_free( VtlMem m );

/** resize resizable memory
\field m handle to memory
\field size new size in bytes
\return true if succeeded
*/
bool vyt_mem_resize( VtlMem m, VytU size );

/** allocated resizable memory address
\field m handle to memory
\return pointer to allocated memory
*/
void * vyt_mem_address( VtlMem m );

/** resizable memory current size
\field m handle to memory
\return current size of memory
*/
VytU vyt_mem_size( VtlMem m );

VYT_CEND()

#endif // VULTOOLSH
