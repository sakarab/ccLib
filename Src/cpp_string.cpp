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
#include "cpp_string.h"

namespace cclib
{
    const char * const CharConstant<char>::crlf = "\r\n";
    const char * const CharConstant<char>::empty_str = "";
    const wchar_t * const CharConstant<wchar_t>::crlf = L"\r\n";
    const wchar_t * const CharConstant<wchar_t>::empty_str = L"";

    std::string narrow_string( const std::wstring& sstr )
    {
        std::string     result;

        utf8::utf16to8( sstr.begin(), sstr.end(), std::back_inserter( result ) );
        return result;
    }

    std::wstring wide_string( const std::string& sstr )
    {
        std::wstring    result;

        utf8::utf8to16( sstr.begin(), sstr.end(), std::back_inserter( result ) );
        return result;
    }

    std_string to_std_string( const std::string& sstr )
    {
#if defined(BOOST_OS_LINUX_AVAILABLE)
        return narrow_string( sstr );
#elif defined (BOOST_OS_WINDOWS_AVAILABLE)
        return wide_string( sstr );
#endif
    }

    std_string to_std_string( const std::wstring& sstr )
    {
#if defined(BOOST_OS_LINUX_AVAILABLE)
        return narrow_string( sstr );
#elif defined (BOOST_OS_WINDOWS_AVAILABLE)
        return wide_string( sstr );
#endif
    }
}
