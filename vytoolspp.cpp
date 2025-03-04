#include "vytools.hpp"
#include <cstdlib>
#include "vytools_impl.h"

namespace vyt {

RefCount::RefCount() {
   rd = NULL;
};

RefCount::RefCount( const RefCount & other ) {
   ref( other.rd );
}

RefCount & RefCount::operator=( const RefCount & other ) {
   ref( other.rd );
   return *this;
}


RefCount::~RefCount() {
   ref( NULL );
}

void RefCount::destroy() {
   delete rd;
   rd = NULL;
}

void RefCount::ref( RefData * nrd ) {
   if ( rd ) {
      if ( ! rd->ref )
         throw new Exc("RefCount mismatch");
      if ( ! --rd->ref )
         destroy();
   }
   if (( rd = nrd ))
      ++ rd->ref;
}


void Cow::write() {
   if ( 1 >= rd->ref ) return;
   ref( copy( rd ) );
}
   
RefData * Cow::copy( RefData * ) {
   Tools::notImpl("copy");
   return NULL;
}

RefData * Cow::wref() {
   write();
   return rd;
}
   
   
InFile::InFile( WString path, bool part ) : part(part) {
   Ptr fh = vytc::vyt_arch_fopen( (const Wide *)path, "r" );
   if ( ! fh ) 
      throw Exc( L"Cannot read file: %s", & path );
   ref( new HRefData( fh ) );
}


InFile::InFile( Ptr handle, bool part ) : part(part) {
   ref( new HRefData( handle ) );
}

Uint InFile::op( Ptr data, Uint n ) {
   if (part) 
      return fread( data, 1, n, (FILE *)handle() );
      else return fread( data, n, 1, (FILE *)handle() );
}

Ptr InFile::handle() { 
   return ((HRefData*)rd)->handle;
}

RefData::RefData() : ref(0) {}

HRefData::HRefData( Ptr handle ) : handle(handle) {}

void InFile::destroy() {
   fclose( (FILE *)handle() );
   Stream::destroy();
}

OutFile::OutFile( WString path ) {
   FILE * fh = vytc::vyt_arch_fopen( path, "w" );
   if ( ! fh ) 
      throw Exc( L"Cannot write file: %s", & path );
   ref( new HRefData( fh ) );
}

void OutFile::destroy() {
   fclose( (FILE *)handle() );
   Stream::destroy();
}

Ptr OutFile::handle() { 
   return ((HRefData*)rd)->handle;
}

Uint OutFile::op( Ptr data, Uint n ) {
   return fwrite( data, n, 1, (FILE *)handle() );
}

void Tools::notImpl( const char * meth ) {
   WString wmeth = meth;
   throw Exc( "Not implemented: %s", & wmeth );
}

void Tools::noIdx() {
   throw Exc( "Array index out of bounds" );
}

Ptr Tools::realloc( Ptr p, Uint size ) {
   Ptr ret = ::realloc( p, size );
   if ( size && ! ret )
      throw Exc("Could not allocate memory");
   return ret;
}  
 
Stream Tools::stdErr() {
   static Stream * ret = NULL;
   if ( ! ret ) ret = new InFile( stderr, true );
   return *ret;
}

Stream Tools::stdIn() {
   static Stream * ret = NULL;
   if ( ! ret ) ret = new InFile( stdin, true );
   return *ret;
}

Stream Tools::stdOut() {
   static Stream * ret = NULL;
   if ( ! ret ) ret = new InFile( stdout, true );
   return *ret;
}

void Tools::debug( WString fmt, ... ) {
   va_list args;
   va_start( args, fmt );
   WString w;
   while ( WString::passArg( fmt, args, w ));
   va_end( args );
   CString ret = w.mb();
   Tools::stdErr().op( (Char *)ret, ret.len() );
}

Uint Stream::op( Ptr data, Uint n ) {
   Tools::notImpl("op");
   return 0;
}

Exc::Exc() {}

Exc::Exc( WString fmt, ... ) {
   msg = "";
   va_list args;
   va_start( args, fmt );
   while ( WString::passArg( fmt, args, msg ) );
   va_end( args );
}
 
}
