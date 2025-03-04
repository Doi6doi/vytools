
#include "vytools.hpp"

using namespace vyt;

int main() {
   WString a = "Hello";
   CString b = "world";
   WString wb(b);
   a = WString::format( "%s, %s!", &a, &wb );
   CString c = a.mb();
   Tools::stdOut().op( (Char *)c, c.len() );
   return 0;
}
