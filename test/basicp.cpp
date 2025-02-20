
#include "vytools.hpp"

using namespace vyt;

int main() {
   WString a = "Hello";
   CString b = "world";
   a = WString::format( "%s, %s!", a, WString(b) );
   CString c = a.mb();
   Tools::stdOut().op( (Char *)c, c.len() );
   return 0;
}
