#include "vytools.hpp"
#include "vytools.h"
#include <cstdlib>
#include "vytools_impl.h"

using namespace vytc;

namespace vyt {

Uint StreamData::op( Ptr data, Uint n ) {
   Tools::notImpl( "op" );
   return 0;
}

struct HStreamData: StreamData {
   Ptr handle;
   VytStreamOp sop;
   HStreamData( Ptr handle, VytStreamOp sop );
   Uint op( Ptr data, Uint n );
};

HStreamData::HStreamData( Ptr handle, VytStreamOp sop )
: handle(handle), sop(sop) { }

Uint HStreamData::op( Ptr data, Uint n ) {
   return sop( handle, data, n );
}

RefCount::RefCount() {
   rd = NULL;
};

RefCount::RefCount( const RefCount & other ) : RefCount() {
   ref( other.rd );
}

RefCount & RefCount::operator=( const RefCount & other ) {
   ref( other.rd );
   return *this;
}


RefCount::~RefCount() {
   ref( NULL );
}

void RefCount::ref( RefData * nrd ) {
   if ( rd ) {
      if ( ! rd->ref )
         throw new Exc("RefCount mismatch");
      if ( ! --rd->ref ) {
         delete rd;
         rd = NULL;
      }
   }
   if (( rd = nrd ))
      ++ rd->ref;
}


RefData * Cow::copy( RefData * ) {
   Tools::notImpl("copy");
   return NULL;
}

RefData * Cow::wref() {
   if ( 1 < rd->ref )
      ref( copy( rd ));
   return rd;
}
      
RefData::RefData() : ref(0) {}

RefData::~RefData() { }

void Tools::notImpl( const char * meth ) {
   WString wmeth = meth;
   throw Exc( "Not implemented: %w", & wmeth );
}

void Tools::noIdx() {
   throw Exc( "Array index out of bounds" );
}

Ptr Tools::realloc( Ptr p, Uint size ) {
   Ptr ret = NULL;
   if ( p || size )
      ret = ::realloc( p, size );
   if ( size && ! ret )
      throw Exc("Could not allocate memory");
   return ret;
}  
 
Stream Tools::stdErr() {
   static Stream ret( *new HStreamData( stderr, vytc::vyt_fwrite ) );
   return ret;
}

Stream Tools::stdIn() {
   static Stream ret( *new HStreamData( stdin, vytc::vyt_fread ));
   return ret;
}

Stream Tools::stdOut() {
   static Stream ret( * new HStreamData( stdout, vytc::vyt_fwrite ) );
   return ret;
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
   return dynamic_cast<StreamData *>(rd)->op( data, n );
}

Stream::Stream( StreamData & data ) {
   ref( & data );
}


Exc::Exc() {}

Exc::Exc( WString fmt, ... ) {
   msg = "";
   va_list args;
   va_start( args, fmt );
   while ( WString::passArg( fmt, args, msg ) );
   va_end( args );
}

WString Exc::message() {
   return msg;
}
 
}
