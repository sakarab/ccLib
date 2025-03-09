//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

// Using Boost test
#define BOOST_TEST_MODULE ccLibTest
#include <boost/test/included/unit_test.hpp>

#include <predef_cc.h>

#include <cpp_string.h>

BOOST_AUTO_TEST_CASE( test_string_view )
{
    BOOST_TEST( cclib::starts_with( cclib::string_view( "A long string" ), cclib::string_view( "A long" ) ) );
    BOOST_TEST( cclib::ends_with( cclib::string_view( "A long string" ), cclib::string_view( "ong string" ) ) );
    //BOOST_TEST( cclib::starts_with( cclib::string_view( "A long string" ), cclib::string_view( "long" ) ) );
    //BOOST_TEST( cclib::ends_with( cclib::string_view( "A long string" ), cclib::string_view( "ong stri" ) ) );
}