#ifndef VYTOOLSIMPLH
#define VYTOOLSIMPLH

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define VYT_NBEGIN() namespace vytc {
#define VYT_NEND() }
#else
#define VYT_NBEGIN()
#define VYT_NEND()
#endif

#define REALLOC( p, type, n ) (type *)realloc( p, (n)*sizeof(type) )

VYT_NBEGIN()

FILE * vyt_arch_fopen( const wchar_t * fname, const char * mode );
char * vyt_arch_tomb( const wchar_t * ws, uint32_t * n );
uint64_t vyt_arch_stamp();

VYT_NEND()

#endif // VYTOOLSIMPLH
