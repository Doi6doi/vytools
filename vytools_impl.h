#ifndef VYTOOLSIMPLH
#define VYTOOLSIMPLH

#include "vytools_defs.h"

VYT_CBEGIN()

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>

VYT_CEND()

#define REALLOC( p, type, n ) (type *)realloc( p, (n)*sizeof(type) )

VYT_NBEGIN()

FILE * vyt_arch_fopen( const wchar_t * fname, const char * mode );
char * vyt_arch_tomb( const wchar_t * ws, uint32_t * n );
uint64_t vyt_arch_stamp();

VYT_NEND()

#endif // VYTOOLSIMPLH
