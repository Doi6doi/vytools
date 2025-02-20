#include "vytools_impl.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

static char * vyaBuf = NULL;
static uint32_t vyaBufSize = 0;

void vyt_arch_die( char * msg ) {
   fprintf( stderr, "%s\n", msg );
   exit(1);
}

char * vyt_arch_grow( uint32_t size ) {
   if ( vyaBufSize < size ) {
      if ( ! ( vyaBuf = REALLOC( vyaBuf, char, size )) )
         vyt_arch_die( "Could not allocate buffer" );
      vyaBufSize = size;
   }
   return vyaBuf;
}

char * vyt_arch_tomb( const wchar_t * ws, uint32_t * m ) {
   uint32_t sz = wcslen( ws )*2;
   while ( true ) {
      char * ret = vyt_arch_grow( sz );
      *m = wcstombs( ret, ws, sz );
      if ( (size_t)-1 == *m )
         vyt_arch_die("Could not convert string");
      if ( *m < sz ) return ret;
      sz += 256;
   }
}
  
FILE * vyt_arch_fopen( const wchar_t * fname, const char * mode ) {
   uint32_t m;
   return fopen( vyt_arch_tomb( fname, &m ), mode );
}

uint64_t vyt_arch_stamp() {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   return (uint64_t)(ts.tv_nsec / 1000000)
      + ((uint64_t)ts.tv_sec * 1000ull);
}
