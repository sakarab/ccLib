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
    std::string     sstr = cclib::Text<char>( original );
    std::string     sstr_2 = cclib::Text<char>( original.cbegin(), original.cend() );

    // vector from string iterators
    StringList      slist_1;
    cclib::Text<char>( slist_1, sstr.begin(), sstr.end() );

    // vector from string iterators
    StringList      slist_2 = cclib::Text<char>( sstr.begin(), sstr.end() );

    // vector from string
    StringList      slist_3;
    cclib::Text<char>( slist_3, sstr );

}
