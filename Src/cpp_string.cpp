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

#include "pre_cc.h"
#include <cpp_string.h>
#include <cc_array.hpp>
#include <cc_memory.hpp>
#include <boost/smart_ptr.hpp>

#if defined (BOOST_OS_WINDOWS_AVAILABLE)

#include <Windows.h>

namespace ccwin
{
    std::string NarrowStringStrict( const std::wstring& str )
    {
        std::string     result;

        if ( str.empty() )
            return result;

        cclib::array<char, 4096>    buffer;
        std::size_t                 hype_len = str.length() * 3 + 1;        // overallocate

        if ( hype_len < buffer.size() )
        {
            int     dest_len = CharFromWChar( str, result, buffer.data(), buffer.size() );

            if ( dest_len >= 0 )
                return std::string( buffer.data(), buffer.data() + dest_len );
        }

        boost::scoped_array<char>   bbuff( new char[hype_len] );
        int                         dest_len = CharFromWChar( str, result, bbuff.get(), hype_len );

        if ( dest_len >= 0 )
            return std::string( bbuff.get(), bbuff.get() + dest_len );
        return std::string();
    }
}
#endif

#if defined (BOOST_OS_LINUX_AVAILABLE)

#include <utf8.h>

namespace cclin
{
    std::string NarrowStringStrict( const std::wstring& str )
    {
        std::string     result;

        utf8::utf16to8( str.begin(), str.end(), std::back_inserter( result ) );
        return result;
    }
}

#endif

namespace cclib
{
    const char * const CharConstant<char>::crlf = "\r\n";
    const char * const CharConstant<char>::empty_str = "";
    const wchar_t * const CharConstant<wchar_t>::crlf = L"\r\n";
    const wchar_t * const CharConstant<wchar_t>::empty_str = L"";

    std::string NarrowStringStrict( const std::wstring& sstr )
    {
#if defined (BOOST_OS_WINDOWS_AVAILABLE)
        return ccwin::NarrowStringStrict( sstr );
#endif
#if defined (BOOST_OS_LINUX_AVAILABLE)
        return cclin::NarrowStringStrict( sstr );
#endif
    }


}
