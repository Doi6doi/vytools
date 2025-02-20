#ifndef VYTOOLSIMPLH
#define VYTOOLSIMPLH

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REALLOC( p, type, n ) (type *)realloc( p, (n)*sizeof(type) )

FILE * vyt_arch_fopen( const wchar_t * fname, const char * mode );
char * vyt_arch_tomb( const wchar_t * ws, uint32_t * n );
uint64_t vyt_arch_stamp();

#ifdef __cplusplus
}
#endif

#endif // VYTOOLSIMPLH
