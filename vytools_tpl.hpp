#ifndef VYTOOLSTPLHPP
#define VYTOOLSTPLHPP

/**
# vytools_tpl.hpp

[VyTools](index) is a *C* and *C++* library which
makes often used tasks easier.

The `vytools_tpl.hpp` is for the template classes of the *C++* part.
All declarations are in the `vyt` namespace.

It contains data class for arrays, vectors, rectangles
and affine transforms. */

/** ## Contents
\toc */

/** ## Details */


#include <new>
#include <cstdarg>

namespace vyt {

/// Data class for reference-counted arrays
template <class T> class ArrayData: public RefData {
protected:
   /// Current size of array
   Uint size;
   /// The stored values
   T * values;
public:

   /** Basic constructor */
   ArrayData() : size(0), values(NULL) {}

   /** Copy constructor */
   ArrayData( const ArrayData<T> & a ) : ArrayData( a.size ) {
      set( 0, a.values, a.size );
   }

   /** Constructor with size
   \param n Array size */
   ArrayData( Uint n ) : ArrayData() { resize(n); }

   /** Destructor */
   ~ArrayData() { resize(0); }

   /** Throws exception if index is out of bounds
   \param i Index */
   void checkIdx( Uint i ) const {
      if ( size <= i ) Tools::noIdx();
   }

   /** Resize array data
   \param n New size */
   void resize( Uint n ) {
      if ( n == size ) return;
      for (Uint i=n; i<size; ++i) (values+i)->~T();
      values = (T *)Tools::realloc( values, n*sizeof(T) );
      for (Uint i=size; i<n; ++i)
         new (values+i) T();
      size = n;
   }

   /** Set values at an index
   \param at Index
   \param ts Pointer to values
   \param n Number of values at `ts` */
   void set( Uint at, const T * ts, Uint n ) {
      if ( n ) checkIdx( at+n-1 );
      for (Uint i=0; i<n; ++i)
         values[at+i] = ts[i];
   }

   /** Pointer to values
   \return Pointer */
   operator T *() { return values; }

   /** Constant pointer to values
   \return Pointer */
   operator const T *() const { return values; }

   /** Current size of array
   \return Size */
   Uint len() const { return size; }
};

/// Reference counted array
template <class T> class Array: public RefCount {
protected:
   /** Pointer to the array data
   \return Pointer */
   ArrayData<T> * vref() { return (ArrayData<T> *)this->ref; }
public:
   /** Cast to pointer */
   operator T*() { return vref().values; }
};

/// 2-dimensional vector
template <class T> class Vec2 {
public:
   /// X coordinate
   T x;
   /// Y coordinate
   T y;

   /** Constructor
   \param x X coordinate
   \param y Y coordinate */
   Vec2( T x, T y ): x(x), y(y) {}
};

/// Rectangle
template <class T> class Rect {
public:
   /// Top cooridnate
   T top;
   /// Left coordinate
   T left;
   /// Width of rectangle
   T width;
   /// Height of rectangle
   T height;

   /** Constructor
   \param top Top cooridnate
   \param left Left coordinate
   \param width Width of rectangle
   \param height Height of rectangle */
   Rect( T top, T left, T width, T height ):
      top(top), left(left), width(width), height(height) {}
};

/// 2-dimensional affine transformation
template <class T> class Trans2 {
public:
   /// X coordinate scale
   T sx;
   /// X coordinate rotate
   T rx;
   /// Y coordinate rotate
   T ry;
   /// Y coordinate scale
   T sy;
   /// X coordinate shift
   T dx;
   /// Y coordinate shift
   T dy;

   /** Constructor
   \param sx X coordinate scale
   \param rx X coordinate rotate
   \param ry Y coordinate rotate
   \param sy Y coordinate scale
   \param dx X coordinate shift
   \param dy Y coordinate shift */
   Trans2( T sx, T rx, T ry, T sy, T dx, T dy ):
      sx(sx), rx(rx), ry(ry), sy(sy), dx(dx), dy(dy) {}
};

/// 2-dimensional vector of [Int](Cpp#Int)
typedef Vec2<Int> IVec2;

/// 2-dimensional vector of [Uint](Cpp#Uint)
typedef Vec2<Uint> UVec2;

/// Rectangle of [Uint](Cpp#Uint)
typedef Rect<Uint> URect;

/// 2-dimensional affine transformation of [Float](Cpp#Float)
typedef Trans2<Float> FTrans2;

}

#endif // VTOOLSTPLHPP
