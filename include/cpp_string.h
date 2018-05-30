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

#ifndef CPP_STRING_H
#define CPP_STRING_H

#if ! defined(CCTEXT)
    #if defined(_UNICODE) || defined(UNICODE)
        #define CCTEXT(a)               L##a
    #else
        #define CCTEXT(a)               a
    #endif
#endif

#ifndef STD_STRING_DEFINED
#define STD_STRING_DEFINED

#include <string>

#if defined(_UNICODE) || defined(UNICODE)
    typedef std::wstring            std_string;
#else
    typedef std::string             std_string;
#endif
typedef std_string::value_type      std_char;

#endif

namespace cclib
{
    /************************************************************
    ********    CharConstant
    ************************************************************/
    template <class CH> struct CharConstant;

    template <> struct CharConstant<char>
    {
        static const char equal = '=';
        static const char minus = '-';
        static const char cr = '\r';
        static const char lf = '\n';
        static const char sp = ' ';
        static const char * const crlf; // = "\r\n";
        static const char * const empty_str; // = "";
        static const char semicolon = ';';
        static const char colon = ':';
        static const char l_bracket = '[';
        static const char r_bracket = ']';
    };

    template <> struct CharConstant<wchar_t>
    {
        static const wchar_t equal = L'=';
        static const wchar_t minus = L'-';
        static const wchar_t cr = L'\r';
        static const wchar_t lf = L'\n';
        static const wchar_t sp = L' ';
        static const wchar_t * const crlf; // = L"\r\n";
        static const wchar_t * const empty_str; // = "";
        static const wchar_t semicolon = L';';
        static const wchar_t colon = L':';
        static const wchar_t l_bracket = L'[';
        static const wchar_t r_bracket = L']';
    };
}

#endif
