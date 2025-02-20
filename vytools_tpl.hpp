#ifndef VYTOOLSTPLHPP
#define VYTOOLSTPLHPP

#include <new>
#include <cstdarg>

namespace vyt {

template <class T> class ArrayData: public RefData {
protected:
   Uint size;
   T * values;
public:
   ArrayData() : size(0), values(NULL) {}
   ArrayData( Uint n ) : ArrayData() { resize(n); }
   ~ArrayData() { resize(0); }
   void resize( Uint n ) {
      if ( n == size ) return;
      for (Uint i=n; i<size; ++i) (values+i)->~T();
      values = (T *)Tools::realloc( values, n*sizeof(T) );
      for (Uint i=size; i<n; ++i) 
         new (values+i) T();
      size = n;
   }
   void set( Uint at, const T * ts, Uint n ) {
      if ( size < at+n ) throw Tools::noIdx();
      for (Uint i=0; i<n; ++i)
         values[at+i] = ts[i];
   }
   operator T *() { return values; }
   operator const T *() const { return values; }
   Uint len() const { return size; }
};

template <class T> class Array: public RefCount {
protected:
   ArrayData<T> * vref() { return (ArrayData<T> *)this.ref; }
public:
   operator T*() { return vref().values; }
};

template <class T> class Vec2 {
public:
   T x, y;
   Vec2( T x, T y ): x(x), y(y) {}
};

template <class T> class Rect {
public:
   T top, left, width, height;
   Rect( T top, T left, T width, T height ):
      top(top), left(left), width(width), height(height) {}
};

template <class T> class Trans2 {
public:
   T sx, rx, ry, sy, dx, dy;
   Trans2( T sx, T rx, T ry, T sy, T dx, T dy ):
      sx(sx), rx(rx), ry(ry), sy(sy), dx(dx), dy(dy) {}
};

typedef Vec2<Int> IVec2;

typedef Vec2<Uint> UVec2;
typedef Rect<Uint> URect;

typedef Trans2<Float> FTrans2;

}

#endif // VTOOLSTPLHPP
