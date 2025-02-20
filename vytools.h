#ifndef VYTOOLSH
#define VYTOOLSH

/// extern C in c++
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

/// number types
typedef uint32_t VytU;
typedef int32_t VytI;
typedef float VytF;
typedef uint64_t VytZ;


/// 2 coordinate vector
#define VYT_VEC2( t ) \
typedef struct Vyt_##t##Vec2 { \
   Vyt##t x, y; \
} * Vyt##t##Vec2;

VYT_VEC2( U )

/// rectangle
#define VYT_RECT( t ) \
typedef struct Vyt_##t##Rect { \
   Vyt##t left, top, width, height; \
} * Vyt##t##Rect;

VYT_RECT( U )

/// 2d transform
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

/// allocated memory
typedef struct Vyt_Mem * VtlMem;

/// stream read or write error
#define VYT_STREAMERR -15001

/// custom read or write operation
typedef VytU (* VytStreamOp)( void * stream, void * mem, VytU size );

/// fread as VtlStreamOp
VytU vyt_fread( void * stream, void * mem, VytU size );
/// fread as VtlStreamOp, can read less than full block
VytU vyt_fread_part( void * stream, void * mem, VytU size );
/// fwrite as VtlStreamOp
VytU vyt_fwrite( void * stream, void * mem, VytU size );
/// write a complete block
bool vyt_write_block( void * stream, VytStreamOp write, void * mem, VytU size );
/// read a complete block
bool vyt_read_block( void * stream, VytStreamOp read, void * mem, VytU size );
/// skip reading bytes
bool vyt_read_skip( void * stream, VytStreamOp read, VytU size );

/// write line to std error
#define vyt_ewrite( ... ) { \
   fprintf( stderr, __VA_ARGS__ ); \
   fprintf( stderr, "\n" ); \
   fflush( stderr ); \
}
   
/// write line to std error and die
#define vyt_die( ... ) { \
   vyt_ewrite( __VA_ARGS__ ); \
   exit(1); \
}

/// string to natural number
bool vyt_nat( VytStr s, VytU * nat );
/// same strings (NULL proof)
bool vyt_same( VytStr a, VytStr b );
/// format string (uses static buffer)
VytStr vyt_sprintf( VytStr fmt, ... );

/// overlapping rect area
// bool vyt_rect_overlap( VtlRect a, VtlRect b );

/// return msec passed since epoch
VytZ vyt_stamp();
/// return msec passed since last call
VytU vyt_stamp_diff();

/// choose cpu as graphical card
int vyt_physical_cpu( void * p );

/// allocate memory
VtlMem vyt_mem_create( VytU size );
/// free memory
void vyt_mem_free( VtlMem m );
/// resize memory
bool vyt_mem_resize( VtlMem m, VytU size );
/// allocated memory address
void * vyt_mem_address( VtlMem );
/// allocated memory size
VytU vyt_mem_size( VtlMem m );

VYT_CEND()

#endif // VULTOOLSH
