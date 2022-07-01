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
#include <cpp_lpstr.h>

namespace
{
    // These are used to call NOT 'const correct' functions.
    /************************************************************
    ********    String Functions (smLPSTR)
    ***********************************************************/
    template<class CH>
    boost::shared_array<typename std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >::value_type>
        LPSTR_in( const std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >& str )
    {
        typedef std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >   str_type;

        typename str_type::size_type                         len = str.length();
        boost::shared_array<typename str_type::value_type>   result = boost::shared_array<typename str_type::value_type>( new typename str_type::value_type[len + 1] );

        std::copy( str.begin(), str.end(), result.get() );
        result[len] = 0;
        return result;
    }
}

namespace cclib
{
    boost::shared_array<std::string::value_type> LPSTR( const std::string& str )        { return LPSTR_in( str ); }
    boost::shared_array<std::wstring::value_type> LPSTR( const std::wstring& str )      { return LPSTR_in( str ); }
}
