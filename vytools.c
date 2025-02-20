#include "vytools.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "vytools_impl.h"

static char * vtBuf = NULL;
static uint32_t vtBufSize = 0;

struct Vyt_Mem {
   uint32_t size;
   void * ptr;
};

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
      vtBuf = vyt_realloc( vtBuf, size );
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

uint32_t vyt_fread( void * stream, void * mem, uint32_t size ) {
   return size * fread( mem, size, 1, stream );
}

uint32_t vyt_fread_part( void * stream, void * mem, uint32_t size ) {
   return fread( mem, 1, size, stream );
}

uint32_t vyt_fwrite( void * stream, void * mem, uint32_t size ) {
   return size * fwrite( mem, size, 1, stream );
}
   
bool vyt_read_block( void * stream, VytStreamOp read, void * mem, uint32_t size ) {
   char * p = mem;
   while ( 0 < size ) {
      uint32_t n = read( stream, mem, size );
      if ( 0 == n ) return false;
      p += n;
      size -=n;
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

bool vyt_write_block( void * stream, VytStreamOp write, void * mem, uint32_t size ) {
   char * p = mem;
   while ( 0 < size ) {
      uint32_t n = write( stream, mem, size );
      if ( 0 == n ) return false;
      p += n;
      size -= n;
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

/// lefoglalt memória címe
void * vyt_mem_address( VtlMem m ) {
   if ( ! m ) return NULL;
   return m->ptr;
}
     
VtlMem vyt_mem_create( uint32_t size ) {
   VtlMem ret = REALLOC( NULL, struct Vyt_Mem, 1 );
   if ( ! ret ) return NULL;
   ret->size = size;
   ret->ptr = REALLOC( NULL, char, size );
   if ( size && ! ret->ptr )
      ret = REALLOC( ret, struct Vyt_Mem, 0 );
   return ret;
}

void vyt_mem_free( VtlMem m ) {
   if ( ! m ) return;
   vyt_mem_resize( m, 0 );
   m = REALLOC( m, struct Vyt_Mem, 0 );
}

bool vyt_mem_resize( VtlMem m, uint32_t size ) {
   void * ptr = REALLOC( m->ptr, char, size );
   if ( size && ! ptr ) return false;
   m->ptr = ptr;
   m->size = size;
   return true;
}

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
