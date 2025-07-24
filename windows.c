
#include "vytools_impl.h"
#include "vytools.h"

VYT_CBEGIN()

#include <windows.h>
#include <stdio.h>

VYT_CEND()

VYT_NBEGIN()

uint64_t vyt_arch_stamp() {
   return GetTickCount();
} 

static char * vyaBuf = NULL;
static uint32_t vyaBufSize = 0;

void vyt_arch_die( const char * msg ) {
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
      if ( -1 == (int)*m )
         vyt_arch_die("Could not convert string");
      if ( *m < sz ) return ret;
      sz += 256;
   }
}

VYT_NEND()
