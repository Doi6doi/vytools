
#include "vytools.hpp"

#include <cstdio>
#include <clocale>

using namespace vyt;

int main() {
try {
   Stream so  = Tools::stdOut();
   CString a("Hello");
   CString b("World");
   CString c = CString::format( "%t, %t!\n", &a, &b );
   c.stream( so );
   CString d("Hello");
   WString e(L"World");
   WString f = WString::format( L"%t, %w!\n", &d, &e );
   WString g = f;
   g[0] = 'K';
   f.mb().stream( so );
   g.mb().stream( so );
   return 0;
} catch ( const Exc & e ) {
   (e.message().mb()+"\n").stream( Tools::stdErr() );
}
}
