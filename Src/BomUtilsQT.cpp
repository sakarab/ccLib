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

#include <pre_cc.h>
#include "BomUtils.h"
#include <string>
#include <fstream>
#include <boost/scope_exit.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "smException.h"

namespace
{

//static char const aaa[] = { 0xFF, 0xFE };
//static char const bbb[] = { 0xFE, 0xFF };
//static char const ccc[] = { 0xEF, 0xBB, 0xBF };
//static char const ddd[] = { 0xFF, 0xFE, 0x00, 0x00 };
//static char const eee[] = { 0x00, 0x00, 0xFF, 0xFE };

inline bool bytes_equal( unsigned char a, unsigned char b )
{
    return ( a == b );
}

bool read_bytes( std::istream& st, unsigned char *buff, int count )
{
    st.read( reinterpret_cast<char *>(buff), count );
    return ( st.gcount() == count );
}

BOM::type GetBom( std::istream& st )
{
    std::istream::pos_type      st_pos = st.tellg();

    BOOST_SCOPE_EXIT( (&st)(st_pos) )
    {
        st.seekg( st_pos );
    }
    BOOST_SCOPE_EXIT_END;

    unsigned char    buff[4];

    std::memset( buff, 0, sizeof(buff) );

    if ( ! read_bytes( st, buff, 2 ) )
        return BOM::no_bom;

    if ( bytes_equal( buff[0], 0xFF ) && bytes_equal( buff[1], 0xFE ) )
        return BOM::utf16le;
    if ( bytes_equal( buff[0], 0xFE ) && bytes_equal( buff[1], 0xFF ) )
        return BOM::utf16be;

    if ( ! read_bytes( st, buff + 2, 1 ) )
        return BOM::no_bom;

    if ( bytes_equal( buff[0], 0xEF ) && bytes_equal( buff[1], 0xBB ) && bytes_equal( buff[2], 0xBF ) )
        return BOM::utf8;

    if ( ! read_bytes( st, buff + 3, 1 ) )
        return BOM::no_bom;

    if ( bytes_equal( buff[0], 0xFF ) && bytes_equal( buff[1], 0xFE ) && bytes_equal( buff[2], 0x00 ) && bytes_equal( buff[3], 0x00 ) )
        return BOM::utf32le;
    if ( bytes_equal( buff[0], 0x00 ) && bytes_equal( buff[1], 0x00 ) && bytes_equal( buff[2], 0xFE ) && bytes_equal( buff[3], 0xFF ) )
        return BOM::utf32be;

    return BOM::no_bom;
}

BOM::type GetBom_in( const char *file_name )
{
    std::ifstream   st( file_name, std::ios_base::in );

    if ( !st )
        throw cclib::BaseException( boost::str( boost::format( "Unable to open file: %1%" ) % file_name ) );
    return GetBom( st );
}

#if defined (WIN32)

BOM::type GetBom_in( const wchar_t *file_name )
{
    std::ifstream   st( file_name, std::ios_base::in );

    return GetBom( st );
}

#endif

} // namespace

#if defined(QT_CORE_LIB)
BOM::type GetBom( const QString& file_name )        { return GetBom_in( file_name.toLocal8Bit().constData() ); }
#endif

BOM::type GetBom( const std::string& file_name )    { return GetBom_in( file_name.c_str() ); }
BOM::type GetBom( const char *file_name )           { return GetBom_in( file_name ); }

#if defined (WIN32)
BOM::type GetBom( const std::wstring& file_name )   { return GetBom_in( file_name.c_str() ); }
BOM::type GetBom( const wchar_t *file_name )        { return GetBom_in( file_name ); }
#endif

/**************************************************************************
********    BOM
**************************************************************************/
//static
wchar_t const * BOM::Names[] =
{
    L"<none>", L"UTF-8", L"UTF-16LE", L"UTF-16BE", L"UTF-32LE", L"UTF-32BE"
};

//static
std::wstring BOM::Name( type bom )
{
    return std::wstring( Names[bom] );
}

//static
BOM::type BOM::Value( std::wstring name )
{
    boost::to_lower( name );
    for ( int n = 1 ; n < static_cast<int>(sizeof(Names) / sizeof(Names[0])) ; ++n )
        if ( std::wcscmp( name.c_str(), Names[n] ) == 0 )
            return BOM::type(n);
    return no_bom;
}
