#include "stdafx.h"

#include <vector>
#include <list>
#include <string>
#include <cpp_string_list.h>

#include "cclib_tests.h"

namespace
{
    using StringViewList = std::vector<std::string_view>;
    using WStringViewList = std::vector<std::wstring_view>;
    using StringVector = std::vector<std::string>;
    using StringList = std::list<std::string>;
    using CharBuffer = std::vector<char>;
}

void testStringList( CRichEditCtrl& logger )
{
    StringVector    original = { "", "", "" };
    StringList      list = { "", "", "" };
    CharBuffer      buffer   = { 'a', 'b', 'c' };

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
