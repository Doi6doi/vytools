#include "vytools.hpp"
#include "vytools_impl.h"
#include <cstring>

using namespace vyt;
using namespace vytc;

CString::CString() {
   ref( new ArrayData<Char>(1) );
}
    
CString::CString( const Char * cs, Uint n ) : CString() {
   if ( 0 == n ) 
      n = CString::lenOf( cs );
   append( cs, n );
}

const ArrayData<Char> & CString::arr() const {
   return *(ArrayData<Char> *)rd;
}

ArrayData<Char> & CString::warr() {
   return *(ArrayData<Char> *)rd;
}

Uint CString::len() const {
   return arr().len()-1;
}

CString::operator Char *() {
   return (Char *)warr();
}

CString::operator const Char *() const {
   return (const Char *)arr();
}

void CString::append( const Char * cs, Uint n ) {
   Uint l = len();
   ArrayData<Char> & a = warr();
   a.resize( l+n+1 );
   a.set( l, cs, n );
   a[l+n] = 0;
}
  
Uint CString::lenOf( const Char * cs ) {
   return strlen( cs );
}


WString::WString() {
   ref( new ArrayData<Wide>(1) );
}


WString::WString( const Char * cs, Uint n ) : WString() {
   if ( 0 == n ) n = CString::lenOf( cs );
   append( cs, n );
}

WString::WString( const Wide * ws, Uint n ) : WString() {
   if ( 0 == n ) n = WString::lenOf( ws );
   append( ws, n );
}

void WString::clear() {
   ArrayData<Wide> & a = warr();
   a.resize(1);
   a[0] = 0;
}

WString & WString::operator =(const Char * cs) {
   clear();
   return operator +=( cs );
}

WString & WString::operator +=(const WString & s) {
   append( (const Wide *)s, s.len() ); 
   return *this;
}

WString & WString::operator +=(const CString & s) {
   append( (const Char *)s, s.len() ); 
   return *this;
}

WString & WString::operator +=(const Char * s) {
   append( s, CString::lenOf(s) );
   return *this;
}

const ArrayData<Wide> & WString::arr() const {
   return *(ArrayData<Wide> *)rd;
}

ArrayData<Wide> & WString::warr() {
   return *(ArrayData<Wide> *)rd;
}

void WString::append( const Wide * ws, Uint n ) {
   Uint l = len();
   ArrayData<Wide> a = warr();
   a.resize( l+n+1 );
   a.set( l, ws, n );
   a[l+n] = 0;
}   

void WString::append( const Char * ws, Uint n ) {
   Uint l = len();
   ArrayData<Wide> a = warr();
   a.resize( l+n+1 );
   a[l+n] = 0;
   Wide * aws = (Wide *)a+l;
   for ( ; 0<n; --n)
      *(aws++) = *(ws++);
}   

bool WString::passArg( WString & fmt, va_list args, WString & dest ) {
   if ( ! fmt ) return false;
   Uint i;
   if ( 2 > fmt.len() || ! fmt.find( '%', i ))
      i = fmt.len();
   if ( 0 < i ) {
      dest += fmt.sub(0,i);
      fmt = fmt.sub(i);
   } else {
      dest += argFmt( fmt[1], args );
      fmt = fmt.sub(2);
   }
   return true;
}

WString WString::argFmt( Wide w, va_list args ) {
   switch (w) {
      case 's': return va_arg( args, WString );
      default: return WString( &w, 1 );
   }
}
 
Uint WString::len() const {
   return arr().len()-1;
}
 
bool WString::find( Wide w, Uint & i ) const {
   const Wide *p = (const Wide *)arr();
   for (i=0; true; ++i, ++p)
      if ( w == *p ) return true;
   return false;
} 

WString WString::sub( Uint at, Uint n ) const {
   Uint l = len();
   if ( l <= at ) return WString();
   if ( 0 == n ) n = l-at;
   return WString( ((const Wide *)arr())+at, n );
}
      
WString::operator const Wide *() const {
   return (const Wide *)arr();
}

Uint WString::lenOf( const Wide * ws ) {
   return wcslen( ws );
}

CString WString::mb() const {
   Uint n;
   Char * ret = vyt_arch_tomb( (const Wide *)arr(), &n );
   if ( ! ret ) throw Exc("Could not convert: '%s'", *this );
   return CString( ret, n );
}

WString WString::format( WString fmt, ... ) {
   WString ret;
   va_list args;
   va_start( args, fmt );
   while ( passArg( fmt, args, ret )) ;
   va_end( args );
   return ret;
}

