
#include "vytools.hpp"

#include <cstdio>
#include <clocale>

using namespace vyt;

int main() {
   auto so  = Tools::stdOut();
   CString a("Hello");
   CString b("World");
   CString c = CString::format( "%t, %t!\n", &a, &b );
   c.stream( so );
   CString d("Hello");
   WString e(L"World");
   WString f = WString::format( L"%t, %w!\n", &d, &e );
   f.mb().stream( so );
   return 0;
}
