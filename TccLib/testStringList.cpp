#include "stdafx.h"

#include <vector>
#include <string>
#include <cpp_StringList.h>

#include "cclib_tests.h"

namespace
{
    using StringList = std::vector<std::string>;


    StringList CreateList()
    {
        return StringList { "", "", "" };
    }
}


void testStringList( ccwin::TRitchEditStreambuf& logger )
{
    StringList      original = CreateList();

    // string from vector
    std::string     sstr = cclib::Text<char>( original );
    // string from vector iterators
    std::string     sstr_2 = cclib::StringFromListIT( original.begin(), original.end() );

    // vector from string iterators
    StringList      slist_1 = cclib::ListFromStringIT( sstr.begin(), sstr.end() );

    // vector from string
    StringList      slist_3 = cclib::Text<char>( sstr );

}
