#include "vytools.hpp"
#include "vytools_impl.h"
#include <cstring>

using namespace vyt;
using namespace vytc;

CString::CString() {
   ref( new ArrayData<Char>(1) );
   warr()[0] = (Char)0;
}
    
CString::CString( const CString & s ) {
   ref( s.rd );
}
    
CString::CString( const Char * cs, Uint n ) : CString() {
   if ( 0 == n ) 
      n = CString::lenOf( cs );
   append( cs, n );
}

const ArrayData<Char> & CString::arr() const {
   return *dynamic_cast<ArrayData<Char> *>(rd);
}

ArrayData<Char> & CString::warr() {
   return *dynamic_cast<ArrayData<Char> *>(rd);
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

const Char & CString::operator []( Uint i ) const {
   const ArrayData<Char> & a = arr();
   a.checkIdx( i );
   return *((const Char *)a+i);
}

Char & CString::operator []( Uint i ) {
   ArrayData<Char> & a = warr();
   a.checkIdx( i );
   return *((Char *)a+i);
}

void CString::append( const Char * cs, Uint n ) {
   Uint l = len();
   ArrayData<Char> & a = warr();
   a.resize( l+n+1 );
   a.set( l, cs, n );
   a[l+n] = (char)0;
}
  
Uint CString::lenOf( const Char * cs ) {
   return strlen( cs );
}

bool CString::operator !() const {
   return ! this->len();
}

CString & CString::operator =( const CString & s) {
   ref( s.rd );
   return *this;
}

CString & CString::operator +=(const CString & s) {
   append( (const Char *)s, s.len() ); 
   return *this;
}


CString CString::operator+( const CString & b ) {
   CString ret(*this);
   return ret += b;
}


CString CString::sub( Uint at, Uint n ) const {
   Uint l = len();
   if ( l <= at ) return CString();
   if ( 0 == n ) n = l-at;
   return CString( ((const Char *)arr())+at, n );
}

bool CString::find( Char c, Uint & i ) const {
   const Char *p = (const Char *)arr();
   Uint l = len();
   for (i=0; i<l; ++i)
      if ( c == p[i] ) return true;
   return false;
} 

CString CString::format( CString fmt, ... ) {
   CString ret;
   va_list args;
   va_start( args, fmt );
   while ( passArg( fmt, args, ret )) ;
   va_end( args );
   return ret;
}

CString CString::argFmt( Char c, va_list args ) {
   switch (c) {
      case 't': return CString( *va_arg( args, CString * ) );
      default: return CString( "%" )+CString(&c,1);
   }
}



bool CString::passArg( CString & fmt, va_list args, CString & dest ) {
   if ( ! fmt ) return false;
   Uint i;
   if ( 2 > fmt.len() || ! fmt.find( '%', i ))
      i = fmt.len();
   if ( 0 < i ) {
      dest += fmt.sub(0,i);
      fmt = fmt.sub(i);
   } else {
      CString a = argFmt( fmt[1], args );
      dest += a;
      fmt = fmt.sub(2);
   }
   return true;
}

Uint CString::stream( Stream s ) {
   return s.op( (Char *)warr(), len() );
}


WString::WString() {
   ref( new ArrayData<Wide>(1) );
   warr()[0] = (Wide)0;
}

WString::WString( const CString & s ) : WString() {
   append( (const Char *)s, s.len() );
}

WString::WString( const WString & s ) {
   ref( s.rd );
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

const Wide & WString::operator []( Uint i ) const {
   const ArrayData<Wide> & a = arr();
   a.checkIdx( i );
   return *((const Wide *)a+i);
}

Wide & WString::operator []( Uint i ) {
   ArrayData<Wide> & a = warr();
   a.checkIdx( i );
   return *((Wide *)a+i);
}

WString & WString::operator =( const WString & s) {
   ref( s.rd );
   return *this;
}

WString & WString::operator +=( const WString & s) {
   append( (const Wide *)s, s.len() );
   return *this;
}

WString WString::operator +(const WString & b ) {
   WString ret(*this);
   return ret += b;
}

bool WString::operator !() const {
   return ! this->len();
}

const ArrayData<Wide> & WString::arr() const {
   return *dynamic_cast<ArrayData<Wide> *>(rd);
}

ArrayData<Wide> & WString::warr() {
   return *dynamic_cast<ArrayData<Wide> *>(wref());
}

void WString::append( const Wide * ws, Uint n ) {
   Uint l = len();
   ArrayData<Wide> & a = warr();
   a.resize( l+n+1 );
   a.set( l, ws, n );
   a[l+n] = (Wide)0;
}   

void WString::append( const Char * cs, Uint n ) {
   Uint l = len();
   ArrayData<Wide> & a = warr();
   a.resize( l+n+1 );
   Wide * ws = (Wide *)a+l;
   for ( ; 0<n; --n) {
      *(ws++) = (Wide)*(cs++);
   }
   *ws = (Wide)0;
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
      WString a = argFmt( fmt[1], args );
      dest += a;
      fmt = fmt.sub(2);
   }
   return true;
}

WString WString::argFmt( Wide w, va_list args ) {
fprintf( stderr, "argFmt %c\n", (Char)w );   
   switch (w) {
      case 't': return WString( *va_arg( args, CString * ) );
      case 'w': return *va_arg( args, WString * );
      default: return WString( "%" )+WString(&w,1);
   }
}
 
Uint WString::len() const {
   return arr().len()-1;
}
 
bool WString::find( Wide w, Uint & i ) const {
   const Wide *p = (const Wide *)arr();
   Uint l = len();
   for (i=0; i<l; ++i)
      if ( w == p[i] ) return true;
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

WString::operator Wide *() {
   return (Wide *)warr();
}
 
Uint WString::lenOf( const Wide * ws ) {
   return wcslen( ws );
}

CString WString::mb() const {
   Uint n;
   const Wide * a = (const Wide *)arr();
fprintf( stderr, "mb a:%p a0:%d\n", a, (int)*a );
   Char * ret = vyt_arch_tomb( a, &n );
   if ( ! ret ) throw Exc("Could not convert: '%w'", this );
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

