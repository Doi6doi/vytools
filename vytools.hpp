#ifndef VYTOOLSHPP
#define VYTOOLSHPP

/**
# vytools.hpp

[VyTools](index) is a *C* and *C++* library which
makes often used tasks easier.

The `vytools.hpp` is for the *C++* part.
It defines the namespace `vyt`. 
All other declarations are in that namespace.

It contains basic types,
some utility functions, base classes for non-create,
non-assign, referent-counted or copy-on-write classes,
exception, a basic stream and a char- and wide string. */

/** ## Contents
\toc */

/** ## Details */

#include <cstdint>
#include <cwchar>

namespace vyt {

/** Typeless pointer
interchangeable with `[VytPtr](C#VytPtr)`*/
typedef void * Ptr;
/** Unsigned 32-bit integer
interchangeable with `[VytU](C#VytU)`*/
typedef uint32_t Uint;
/** Signed 32-bit integer
interchangeable with `[VytI](C#VytI)`*/
typedef int32_t Int;
/** Unsigned 64-bit integer
interchangeable with `[VytZ](C#VytZ)`*/
typedef uint64_t Zint;
/** 32-bit floating point value
interchangeable with `[VytF](C#VytF)`*/
typedef float Float;

/// 8-bit character
typedef char Char;
/// Unicode character
typedef wchar_t Wide;


class CString;
class WString;
class Exc;
class Stream;

/// Utility functions
struct Tools {
   /** Throws exception for index out of bounds */
   static void noIdx();
   /** Throws exception if a method is not implemented
   \param meth The method name */
   static void notImpl( const char * meth );
   /** Memory reallocation
   \param old Previous memory address or `NULL`
   \param size New memory size
   \return Pointer to allocated memory or `NULL` if `size` is `0`
   \throws `Exc` if allocation fails */
   static Ptr realloc( Ptr old, Uint size );
   /** Write formatted string to `stderr`
   \param fmt Format string as in [printf](https://en.cppreference.com/w/c/io/fprintf)
   \param ... Arguments as in [printf](https://en.cppreference.com/w/c/io/fprintf)*/
   static void debug(WString fmt, ... );
   /** Standard input
   \return Stream handle */
   static Stream stdIn();
   /** Standard output
   \return Stream handle */
   static Stream stdOut();
   /** Standard error
   \return Stream handle */
   static Stream stdErr();
};

/** No-create class 
(e.g singleton) */
class NoCreate {
protected:
   /** Hidden default constructor */
   NoCreate();
};

/** No-assignment class */
class NoAssign {
protected:   
   /** Hidden copy constructor */
   NoAssign( const NoAssign & );
   /** Hidden assignment */
   NoAssign & operator = (const NoAssign & other);
public:
   NoAssign() {}
};

/** Reference counted data
Base class to be used with `[#RefCount]` */
struct RefData: public NoAssign {
public:
   /// Number of references to this data
   Uint ref;
   /// Default constructor 
   RefData();
   /// Virtual destructor
   virtual ~RefData();
};
   
/** Reference counted class */
class RefCount {
protected:
   /// Reference to the real data stored in a `[#RefData]`
   RefData * rd;
   /// Default constructor
   RefCount();
   /** Copy constructor */
   RefCount( const RefCount & );
   /** Assignment
   Increases the reference */
   RefCount & operator=( const RefCount & );
   /** change reference to `newRd`
   \param newRd The new referenced data. Can be `NULL` */
   virtual void ref( RefData * newRd );
public:
   /** Destructor
   Decreases the reference */
   ~RefCount();
};

/// copy-on-write refcounted object
class Cow: public RefCount {
protected:
   virtual RefData * copy( RefData * );
   RefData * wref();
};
 
}

#include "vytools_tpl.hpp"

namespace vyt {

class CString: public Cow {
protected:
   RefData * copy( RefData * );
   ArrayData<Char> & warr();
   const ArrayData<Char> & arr() const;
   void append( const Char *, Uint n );   
   void clear();
public:
   CString();
   CString( const CString & );
   CString( const Char *, Uint n = 0 );
   operator const Char *() const;
   operator Char *();
   const Char & operator[]( Uint ) const;
   Char & operator[]( Uint );
   CString & operator =( const CString & );
   CString & operator +=( const CString & );
   CString operator+( const CString & );
   bool operator !() const;
   Uint len() const;
   bool find( Char, Uint & ) const;
   CString sub(Uint at, Uint n=0) const;
   Uint stream( Stream s );
public:
   static Uint lenOf( const Char * );   
   static CString format( CString fmt, ... );   
   static bool passArg( CString & fmt, va_list, CString & dest );
   static CString argFmt( Char, va_list ); 
};


class WString: public Cow  {
protected:
   RefData * copy( RefData * );
   ArrayData<Wide> & warr();
   const ArrayData<Wide> & arr() const;
   void append( const Wide *, Uint );
   void append( const Char *, Uint );
   void clear();
public:
   WString();
   WString( const WString & );
   WString( const CString & );
   WString( const Wide *, Uint n = 0 );
   WString( const Char *, Uint n = 0 );
   operator const Wide *() const;
   operator Wide *();
   WString & operator =( const WString & );
   WString & operator +=( const WString & );
   WString operator+( const WString & b );
   bool operator !() const;
   const Wide & operator[]( Uint ) const;
   Wide & operator[]( Uint );
   Uint len() const;
   bool find( Wide, Uint & ) const;
   WString sub(Uint at, Uint n=0) const;
   /// multi-byte encoding
   CString mb() const;
   /// read or write to string
   Uint stream( Stream s );
public:
   static Uint lenOf( const Wide * );   
   static WString format( WString fmt, ... );   
   static bool passArg( WString & fmt, va_list, WString & dest );
   static WString argFmt( Wide, va_list ); 
};


class Exc {
protected:
   WString msg; 
public:
   Exc();
   Exc( WString fmt, ... );
   WString message() const;
};

class Excs {
};   

/// stored data of a stream
struct StreamData: public RefData {
   virtual Uint op( Ptr data, Uint n );
};

/// stream read or write class
class Stream: public RefCount {
public:
   /** Construct with handle and operation
   \param data The stream data */
   Stream( StreamData & data );
   /// read or write 
   Uint op( Ptr data, Uint n );
   Stream & operator =( const Stream & );
};

enum class FileMode { READ, READPART, WRITE };

/// file manipulator class
class File {
public:
   static Stream open( WString path, FileMode );
};

}

#endif // VYTOOLSHPP
