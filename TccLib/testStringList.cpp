#include "stdafx.h"

#include <vector>
#include <string>
#include <cpp_StringList.h>

#include "cclib_tests.h"

namespace
{
    using StringList = std::vector<std::string>;
    using CharBuffer = std::vector<char>;

    StringList CreateList()
    {
        return StringList { "", "", "" };
    }

    CharBuffer CreateBuffer()
    {
        return CharBuffer { 'a', 'b', 'c' };
    }

}


void testStringList( CRichEditCtrl& logger )
{
    StringList      original = CreateList();
    CharBuffer      buffer = CreateBuffer();

    // string from vector<string>
    std::string     sstr = cclib::Text<char>( original );

    // string from vector<string>::iterator
    std::string     sstr_2 = cclib::StringFromListIT( original.begin(), original.end() );

    // vector from any CHAR iterator
    StringList      slist_1 = cclib::ListFromCharIT( sstr.cbegin(), sstr.cend() );
    StringList      slist_2 = cclib::ListFromCharIT( buffer.cbegin(), buffer.cend() );

    // vector from string
    StringList      slist_3 = cclib::Text<char>( sstr );

}
