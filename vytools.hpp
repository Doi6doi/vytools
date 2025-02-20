#ifndef VYTOOLSHPP
#define VYTOOLSHPP

#include <cstdint>
#include <cwchar>

namespace vyt {

/// basic types
typedef void * Ptr;
typedef int32_t Int;
typedef uint32_t Uint;
typedef uint64_t Zint;
typedef float Float;

typedef char Char;
typedef wchar_t Wide;

class CString;
class WString;
class Exc;
class Stream;

/// useful calls
struct Tools {
   /// exception for index out of bounds
   static Exc noIdx();
   /// exception if a method is not implemented
   static Exc notImpl( const char * meth );
   /// memory reallocation
   static Ptr realloc( Ptr old, Uint size );
   /// write to std out
   static void debug(WString fmt, ... );
   /// standard input
   static Stream stdIn();
   /// standard output
   static Stream stdOut();
   /// standard error
   static Stream stdErr();
};

/// no-create class (e.g singleton)
class NoCreate {
protected:
   NoCreate();
};

/// no-assignment class
class NoAssign {
protected:   
   NoAssign & operator = (const NoAssign &);
};

/// reference counted data
struct RefData {
   Uint ref;
   RefData();
};

/// refdata with a single handle
struct HRefData: RefData {
   Ptr handle;
   HRefData( Ptr handle );
};
   
/// reference counted class
class RefCount {
protected:
   RefCount();
   RefData * rd;
   virtual void ref( RefData * );
   virtual void destroy();
public:
   RefCount( const RefCount & );
   ~RefCount();
   RefCount & operator=( const RefCount & );
};

/// refcounted handle
class HRefCount: public RefCount {
protected:
   Ptr handle();
};

/// copy-on-write refcounted object
class Cow: public RefCount {
protected:
   void write();
   RefData * copy( RefData * );
   RefData * wref();
};
  
  
/// stream read or write class
class Stream: public RefCount {
public:
   /// read or write 
   virtual Uint op( Ptr data, Uint n );
};

}

#include "vytools_tpl.hpp"

namespace vyt {

class CString: public Cow {
protected:
   ArrayData<Char> & warr();
   const ArrayData<Char> & arr() const;
   void append( const Char *, Uint n );   
   void clear();
public:
   CString();
   CString( const Char *, Uint n = 0 );
   operator const Char *() const;
   operator Char *();
   Uint len() const;
public:
   static Uint lenOf( const Char * );   
   static CString format( CString fmt, ... );   
   static bool passArg( CString & fmt, va_list, CString & dest );
   static CString argFmt( Char, va_list ); 
};

class WString: public Cow  {
protected:
   ArrayData<Wide> & warr();
   const ArrayData<Wide> & arr() const;
   void append( const Wide *, Uint );
   void append( const Char *, Uint );
   void clear();
public:
   WString();
   WString( const Wide *, Uint n = 0 );
   WString( const Char *, Uint n = 0 );
   operator const Wide *() const;
   WString & operator +=( const WString & );
   WString & operator +=( const CString & );
   WString & operator +=( const Char * );
   WString & operator +=( const Wide * );
   WString & operator =( const CString & );
   WString & operator =( const Char * );
   WString & operator =( const Wide * );
   Uint len() const;
   bool find( Wide, Uint & ) const;
   WString sub(Uint at, Uint n=0) const;
   /// multi-byte encoding
   CString mb() const;
public:
   static Uint lenOf( const Wide * );   
   static WString format( WString fmt, ... );   
   static bool passArg( WString & fmt, va_list, WString & dest );
   static WString argFmt( Wide, va_list ); 
};

template<class T> WString operator+( const WString &a, const T & b ) {
   WString ret(a); return ret += b; }
template<class T> WString operator+( const WString &a, const T * b ) {
   WString ret(a); return ret += b; }

class Exc {
protected:
   WString msg; 
public:
   Exc();
   Exc( WString fmt, ... );
};

/// file reader class
class InFile: public Stream {
protected:
   bool part;
   void destroy();
public:   
   InFile( WString, bool part=false );
   InFile( Ptr, bool part=false );
   Ptr handle();
   Uint op( Ptr data, Uint n );
};

/// file writer class
class OutFile: public Stream {
protected:
   void destroy();
public:   
   OutFile( WString );
   OutFile( Ptr );
   Ptr handle();
   Uint op( Ptr data, Uint n );
};

}

#endif // VYTOOLSHPP
