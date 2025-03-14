VYT_CBEGIN()

#include "vytools_impl.h"
#include <windows.h>
#include <stdio.h>

VYT_CEND()

VYT_NBEGIN()

VtlZ vtl_arch_stamp( VcpStr msg ) {
   static DWORD last = 0;
   DWORD curr = GetTickCount();
   if ( last )
      fprintf( stderr, "%s: %ld\n", msg, curr-last );
   last = curr;
} 

VYT_NEND()
