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

#include <predef_cc.h>

// the STD_STRING_DEFINED macro is for interoperability with pplib library
// a library used for a single projects
#ifndef STD_STRING_DEFINED
#define STD_STRING_DEFINED

#include <string>
#include <cstring>
#include "utf8.h"

#if defined(_UNICODE) || defined(UNICODE)
    typedef std::wstring            std_string;
#else
    typedef std::string             std_string;
#endif
typedef std_string::value_type      std_char;

#endif

#if !defined( CC_HAVE_STRING_VIEW )
    #include <boost/utility/string_view.hpp>
    #include <boost/utility/string_view_fwd.hpp>
#else
    #include <string_view>
#endif

namespace cclib
{
#ifdef CC_HAVE_STRING_VIEW
    typedef std::string_view        string_view;
    typedef std::wstring_view       wstring_view;
#else
    typedef boost::string_view      string_view;
    typedef boost::wstring_view     wstring_view;
#endif

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

    template <class T> struct stru      {};

    template <> struct stru<char>
    {
        typedef     char        char_type;

        static char *strcpy( char *dest, const char *src )                      { return std::strcpy( dest, src ); }
        static size_t strlen( const char *s )                                   { return std::strlen( s ); }
        static const char *strrchr( const char *s, int c )                      { return std::strrchr( s, c ); }
        static char *strrchr( char *s, int c )                                  { return std::strrchr( s, c ); }
        static int strcmp( const char *s1, const char *s2 ) 	                { return std::strcmp( s1, s2 ); }
        static int stricmp( const char *s1, const char *s2 )
        {
#if defined (BOOST_COMP_MSVC_AVAILABLE)
            return _stricmp( s1, s2 );
#else // defined (BOOST_COMP_GNUC_AVAILABLE)
            return strcasecmp( s1, s2 );
#endif
        }
        static int strncmp( const char *s1, const char *s2, size_t maxlen )     { return std::strncmp( s1, s2, maxlen ); }
        static int strnicmp( const char *s1, const char *s2, size_t maxlen )
        {
#if defined (BOOST_COMP_MSVC_AVAILABLE)
            return _strnicmp( s1, s2, maxlen );
#else // defined (BOOST_COMP_GNUC_AVAILABLE)
            return strncasecmp( s1, s2, maxlen );
#endif
        }
    };

    template <> struct stru<wchar_t>
    {
        typedef     wchar_t     char_type;

        static wchar_t *strcpy( wchar_t *dest, const wchar_t *src )                 { return std::wcscpy( dest, src ); }
        static size_t strlen( const wchar_t *s )                                    { return std::wcslen( s ); }
        static const wchar_t *strrchr( const wchar_t *s, wchar_t c )                { return std::wcsrchr( s, c ); }
        static wchar_t *strrchr( wchar_t *s, wchar_t c )                            { return std::wcsrchr( s, c ); }
        static int strcmp( const wchar_t *s1, const wchar_t *s2 )                   { return std::wcscmp( s1, s2 ); }
        static int stricmp( const wchar_t *s1, const wchar_t *s2 )
        {
#if defined (BOOST_COMP_MSVC_AVAILABLE)
            return _wcsicmp( s1, s2 );
#else // defined (BOOST_COMP_GNUC_AVAILABLE)
            return wcscasecmp( s1, s2 );
#endif
        }
        static int strncmp( const wchar_t *s1, const wchar_t *s2, size_t maxlen )   { return std::wcsncmp( s1, s2, maxlen ); }
        static int strnicmp( const wchar_t *s1, const wchar_t *s2, size_t maxlen )
        {
#if defined (BOOST_COMP_MSVC_AVAILABLE)
            return _wcsnicmp( s1, s2, maxlen );
#else // defined (BOOST_COMP_GNUC_AVAILABLE)
            return wcsncasecmp( s1, s2, maxlen );
#endif
        }
    };

    // Reminder: LFLF, CRCR and LFCR are **NOT** valid combinations
    // on entry 'ch' is assumed to be CR or LF
    template <class IT, class CH> void AdvanceOverCRLF( IT& it, IT end, CH ch )
    {
        if ( it != end )
        {
            ++it;
            if ( ch == CharConstant<CH>::cr && it != end )
                if ( *it == CharConstant<CH>::lf )
                    ++it;
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
