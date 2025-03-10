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

// Back off using precompiled headers
// give it more time to understand the framework
// #include "pre_TestCmd.h"

#define BOOST_TEST_MODULE ccLibTest
#include <boost/test/included/unit_test.hpp>

#include <predef_cc.h>

#include <cpp_string.h>

// TODO: Do the 'BOOST_AUTO_TEST_TEMPLATE_CASE'
BOOST_AUTO_TEST_CASE( test_string_view )
{
    BOOST_TEST( cclib::starts_with( cclib::string_view( "A long string" ), cclib::string_view( "A long" ) ) );
    BOOST_TEST( cclib::ends_with( cclib::string_view( "A long string" ), cclib::string_view( "ong string" ) ) );
    BOOST_TEST( cclib::starts_with( cclib::string_view( "A long string" ), cclib::string_view( "long" ) ) == false );
    BOOST_TEST( cclib::ends_with( cclib::string_view( "A long string" ), cclib::string_view( "ong stri" ) ) == false );
    BOOST_TEST( cclib::contains( cclib::string_view( "A long string" ), cclib::string_view( "ong stri" ) ) );

    BOOST_TEST( cclib::starts_with( std::string( "A long string" ), std::string( "A long" ) ) );
    BOOST_TEST( cclib::ends_with( std::string( "A long string" ), std::string( "ong string" ) ) );
    BOOST_TEST( cclib::starts_with( std::string( "A long string" ), std::string( "long" ) ) == false );
    BOOST_TEST( cclib::ends_with( std::string( "A long string" ), std::string( "ong stri" ) ) == false );
    BOOST_TEST( cclib::contains( std::string( "A long string" ), std::string( "ong stri" ) ) );

    BOOST_TEST( cclib::starts_with( cclib::wstring_view( L"A long string" ), cclib::wstring_view( L"A long" ) ) );
    BOOST_TEST( cclib::ends_with( cclib::wstring_view( L"A long string" ), cclib::wstring_view( L"ong string" ) ) );
    BOOST_TEST( cclib::starts_with( cclib::wstring_view( L"A long string" ), cclib::wstring_view( L"long" ) ) == false );
    BOOST_TEST( cclib::ends_with( cclib::wstring_view( L"A long string" ), cclib::wstring_view( L"ong stri" ) ) == false );
    BOOST_TEST( cclib::contains( cclib::wstring_view( L"A long string" ), cclib::wstring_view( L"ong stri" ) ) );

    BOOST_TEST( cclib::starts_with( std::wstring( L"A long string" ), std::wstring( L"A long" ) ) );
    BOOST_TEST( cclib::ends_with( std::wstring( L"A long string" ), std::wstring( L"ong string" ) ) );
    BOOST_TEST( cclib::starts_with( std::wstring( L"A long string" ), std::wstring( L"long" ) ) == false );
    BOOST_TEST( cclib::ends_with( std::wstring( L"A long string" ), std::wstring( L"ong stri" ) ) == false );
    BOOST_TEST( cclib::contains( std::wstring( L"A long string" ), std::wstring( L"ong stri" ) ) );
}
