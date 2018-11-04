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

// All projects are Unicode.
// All portable (Linux - Windows) programs use Qt or wxWidgets

// according to QString documentation the constructor QString( char * ) assumes that the parameter
// is a UTF-8 encoded string so although it is a bit time consuming it is generally acceptable.
// so the macro CCTEXT is OK.
#if ! defined(CCTEXT)
    #if defined(_UNICODE) || defined(UNICODE)
        #define CCTEXT(a)               L##a
    #else
        #define CCTEXT(a)               a
    #endif
#endif

// the STD_STRING_DEFINED macro is for interoperability with pplib library
// a library used for a single projects
#ifndef STD_STRING_DEFINED
#define STD_STRING_DEFINED

#include <string>
#include "utf8.h"

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

    // Reminder: LFLF, CRCR and LFCR are **NOT** valid combinations
    // on entry 'ch' is assumed to be CR or LF
    template <class IT, class CH> void AdvanceOverCRLF( IT& it, IT end, CH ch )
    {
        if ( it != end )
        {
            ++it;
            if ( ch == CharConstant<CH>::cr && it != end )
            {
                CH      ch_2 = *it;

                if ( ch == CharConstant<CH>::lf )
                    ++it;
            }
        }
    }

    // to_std_string
    std_string to_std_string( const std::string& sstr );
    std_string to_std_string( const std::wstring& sstr );

    // to std::string - parameter is typically std_string
    inline std::string narrow_string( const std::string& sstr )         { return sstr; }
    std::string narrow_string( const std::wstring& sstr );
    std::wstring wide_string( const std::string& sstr );
    inline std::wstring wide_string( const std::wstring& sstr )         { return sstr; }

}

#endif
