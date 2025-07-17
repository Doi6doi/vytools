#include "vytools.h"

VYT_CBEGIN()

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

VYT_CEND()

#include "vytools_impl.h"

VYT_NBEGIN()

#define FRAMESTEP 64

static char * vtBuf = NULL;
static uint32_t vtBufSize = 0;

typedef struct Vyt_Frame {
   struct Vyt_Frame * prev;
   uint32_t size;
   uint32_t count;
   VytPtr * ptrs;
} * VytFrame;

static VytFrame frame = NULL;

#define VYT_V2ADDB( t ) \
VYT_V2ADD( t ) { \
   struct Vyt_##t##Vec2 ret = { .x = a->x + b->x, .y = a->y + b->y }; \
   return ret; \
}

VYT_V2ADDB( I )
VYT_V2ADDB( U )
VYT_V2ADDB( F )

#define VYT_V2SUBB( t ) \
VYT_V2SUB( t ) { \
   struct Vyt_##t##Vec2 ret = { .x = a->x - b->x, .y = a->y - b->y }; \
   return ret; \
}

VYT_V2SUBB( I )
VYT_V2SUBB( U )
VYT_V2SUBB( F )

void * vyt_realloc( void * old, size_t size ) {
   void * p = realloc( old, size );
   if ( 0 < size && ! p )
      vyt_die( "Out of memory" );
   return p;
}

bool vyt_nat( VytStr s, VytU * i ) {
   *i = 0;
   VytStr t = s;
   while ( *t ) {
      if ( '0' <= *t && *t <= '9' )
         *i = *i * 10 + *t - '0';
      else
         return false;
      ++t;
   }
   return t != s;
}

char * vyt_grow( uint32_t size ) {
   if ( vtBufSize < size ) {
      vtBuf = (char *)vyt_realloc( vtBuf, size );
      vtBufSize = size;
   }
   return vtBuf;
}

VytStr vyt_cat( VytStr a, VytStr b ) {
   int la = strlen( a );
   int lb = strlen( b );
   vyt_grow( la+lb+1 );
   vtBuf[0] = 0;
   strcat( vtBuf, a );
   strcat( vtBuf, b );
   return vtBuf;
}

bool vyt_same( VytStr a, VytStr b ) {
   if ( !a ) return !b;
   if ( !b ) return false;
   return 0 == strcmp( a, b );
}

VytU vyt_fread( void * stream, void * mem, uint32_t size ) {
   return size * fread( mem, size, 1, (FILE *)stream );
}

uint32_t vyt_fread_part( void * stream, void * mem, uint32_t size ) {
   return fread( mem, 1, size, (FILE *)stream );
}

uint32_t vyt_fwrite( void * stream, void * mem, uint32_t size ) {
   return size * fwrite( mem, size, 1, (FILE *)stream );
}

 
bool vyt_block_op( void * stream, VytStreamOp op, void * mem, uint32_t size ) {
   char * p = (char *)mem;
   while ( 0 < size ) {
      uint32_t n = op( stream, mem, size );
      if ( 0 == n ) return false;
      p += n;
      size -= n;
   }
   return true;
}

bool vyt_read_skip( void * stream, VytStreamOp read, uint32_t size ) {
   static char buf[512];
   while ( 0 < size ) {
      uint32_t n = size <= 512 ? size : 512;
      n = read( stream, buf, n );
      if ( 0 == n ) return false;
      size -=n;
   }
   return true;
}

/// átfedő intervallumok
/*static bool vtl_iv_overlap( int32_t a, uint32_t al, int32_t b, uint32_t bl ) {
   return (a <= b && b < a+al)
      || (b <= a && a < b+bl);
}
*/
/// átfedő téglalapok
/*bool vtl_rect_overlap( VtlRect a, VtlRect b ) {
   return vtl_iv_overlap( a->left, a->width, b->left, b->width )
      && vtl_iv_overlap( a->top, a->height, b->top, b->height );
}
*/


VytStr vyt_sprintf( VytStr fmt, ... ) {
   va_list args;
   va_start( args, fmt );
   int n = vsnprintf( vtBuf, vtBufSize, fmt, args );
   va_end( args );
   if ( 0 > n ) {
      vyt_grow( 1 );
      vtBuf[0] = 0;
   } else if ( vtBufSize < n+1 ) {
      vyt_grow( n+1 );
      va_start( args, fmt );
      vsnprintf( vtBuf, vtBufSize, fmt, args );
      va_end( args );
   }
   return vtBuf;
}

VytZ vyt_stamp() {
   return vyt_arch_stamp();
}

VytU vyt_stamp_diff() {
   static VytZ old = 0;
   VytZ now = vyt_arch_stamp();
   VytU ret = old ? now-old : 0;
   old = now;
   return ret;
}   

void vyt_frame( bool openNew ) {
   if ( openNew ) {
      VytFrame ret = (VytFrame)realloc( NULL, sizeof( struct Vyt_Frame ));
      if ( ! ret )
         vyt_die("Could not allocate new frame");
      ret->prev = frame;
      ret->size = 0;
      ret->count = 0;
      ret->ptrs = NULL;
      frame = ret;
   } else {
      if ( ! frame )
         vyt_die("No frame to pop");
      for (int i=0; i<frame->count; ++i) 
         frame->ptrs[i] = realloc( frame->ptrs[i], 0 );
      frame->ptrs = (VytPtr *)realloc( frame->ptrs, 0 );
      VytFrame old = frame;
      frame = frame->prev;
      old = (VytFrame)realloc( old, 0 );
   }
}

VytPtr vyt_alloc( VytPtr old, VytU size ) {
   if ( ! ( old || size ))
      return NULL;
   VytPtr ret = realloc( old, size );
   if ( size && ! ret )
      vyt_die("Could not allocate memory");
   if ( ! frame ) return ret;
   if ( old ) {
      for ( int i=0; i<frame->count; ++i) {
         if ( frame->ptrs[i] == old ) {
            if ( ret ) {
               frame->ptrs[i] = ret;
            } else {
               frame->ptrs[i] = frame->ptrs[frame->count-1];
               --frame->count;
            }
         }
      }
   } else if ( ret ) {
      if ( frame->count == frame->size ) {
         frame->size += FRAMESTEP;
         if ( ! ( frame->ptrs = (VytPtr *)realloc( 
               frame->ptrs, frame->size * sizeof( VytPtr ))))
            vyt_die("Could not allocate memory");
      }
      frame->ptrs[frame->count] = ret;
      ++frame->count;
   }
   return ret;
}

VytU vyt_mread( VytStream stream, VytPtr mem, VytU size ) {
   memcpy( mem, *(VytPtr *)stream, size );
   return size;
}


VytU vyt_mwrite( VytStream stream, VytPtr mem, VytU size ) {
   memcpy( *(VytPtr *)stream, mem, size );
   return size;
}


VYT_NEND()
