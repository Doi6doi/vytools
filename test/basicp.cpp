#include <vytools.hpp>

using namespace vyt;
 
int main() {
   Ptr p = NULL;
   p = Tools::realloc( p, 20 );
   p = Tools::realloc( p, 0 );
   WString ws = "Hello";
   CString cs = "world";
   Tools::debug(ws+" "+cs );
   return 0;
}
