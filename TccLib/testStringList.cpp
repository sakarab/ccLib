#include "stdafx.h"

#include <vector>
#include <list>
#include <string>
#include <cpp_string_list.h>

#include "cclib_tests.h"

namespace
{
    typedef std::vector<cclib::string_view>     StringViewList;
    typedef std::vector<cclib::wstring_view>    WStringViewList;
    typedef std::vector<std::string>            StringVector;
    typedef std::list<std::string>              StringList;
    typedef std::vector<char>                   CharBuffer;
}

void testStringList( CRichEditCtrl& logger )
{
#if defined(CC_HAVE_INITIALIZER_LIST)
    StringVector    original = { "", "", "" };
    StringList      list = { "", "", "" };
    CharBuffer      buffer   = { 'a', 'b', 'c' };
#else
    StringVector    original;
    StringList      list;
    CharBuffer      buffer;
#endif

    // string from vector<string>
    std::string     sstr = cclib::ToString( original );

    sstr = cclib::ToString( original, 'p' );

    // string from vector<string>::iterator
    sstr = cclib::ToString( original.begin(), original.end() );
    sstr = cclib::ToString( original.cbegin(), original.cend() );
    sstr = cclib::ToString( original.begin(), original.end(), 'p' );
    sstr = cclib::ToString( original.cbegin(), original.cend(), 'p' );

    // vector from any CHAR iterator
    StringVector    vvec = cclib::ToList<StringVector>( sstr.begin(), sstr.end() );

    vvec = cclib::ToList<StringVector>( sstr.begin(), sstr.end(), 'p' );
    vvec = cclib::ToList<StringVector>( sstr.cbegin(), sstr.cend() );
    vvec = cclib::ToList<StringVector>( sstr.cbegin(), sstr.cend(), 'p' );

    // vector from string
    vvec = cclib::ToList<StringVector>( sstr );
}
