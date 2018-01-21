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

#pragma once

#ifndef CCLIB_WIN_STR_H
#define CCLIB_WIN_STR_H

#include <Windows.h>
#include "cpp_string.h"
#include <functional>
#include <boost/smart_ptr.hpp>

namespace ccwin
{
    /************************************************************
    ********    case_insensitive_string_compare
    ***********************************************************/
    template<class CH> struct case_insensitive_string_compare_ptr;
    template<class CH> class case_insensitive_string_compare_ptr_n;

    template<>
    struct case_insensitive_string_compare_ptr<wchar_t> : public std::binary_function<const wchar_t *, const wchar_t *, int>
    {
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return _wcsicmp( str1, str2 );
        }
    };

    template<>
    struct case_insensitive_string_compare_ptr<char> : public std::binary_function<const char *, const char *, int>
    {
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return _stricmp( str1, str2 );
        }
    };

    template<>
    class case_insensitive_string_compare_ptr_n<wchar_t> : public std::binary_function<const wchar_t *, const wchar_t *, int>
    {
    private:
        size_t  mMaxChars;
    public:
        explicit case_insensitive_string_compare_ptr_n( size_t max_chars ) : mMaxChars( max_chars ) {}
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return _wcsnicmp( str1, str2, mMaxChars );
        }
    };

    template<>
    class case_insensitive_string_compare_ptr_n<char> : public std::binary_function<const char *, const char *, int>
    {
    private:
        size_t  mMaxChars;
    public:
        explicit case_insensitive_string_compare_ptr_n( size_t max_chars ) : mMaxChars( max_chars ) {}
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return _strnicmp( str1, str2, mMaxChars );
        }
    };
    //----------------------------------------------------------------
    template<class CH> class case_insensitive_string_compare
        : public std::binary_function<std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      int>
    {
    private:
        case_insensitive_string_compare_ptr<CH>     mCmp;
    public:
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return mCmp( str1.c_str(), str2.c_str() );
        }
    };

    template<class CH> class case_insensitive_string_compare_n
        : public std::binary_function<std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      int>
    {
    private:
        case_insensitive_string_compare_ptr_n<CH>       mCmp;
    public:
        explicit case_insensitive_string_compare_n( size_t max_chars ) : mCmp( max_chars ) {}
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return mCmp( str1.c_str(), str2.c_str() );
        }
    };
    //----------------------------------------------------------------
    template<class CH> struct case_insensitive_string_less_ptr : public std::binary_function<const CH *, const CH *, bool>
    {
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return case_insensitive_string_compare_ptr<CH>()( str1.c_str(), str2.c_str() ) < 0;
        }
    };

    template<class CH> struct case_insensitive_string_less
        : public std::binary_function<std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >,
                                      bool>
    {
        result_type operator()( const first_argument_type& str1, const second_argument_type& str2 ) const
        {
            return case_insensitive_string_compare_ptr<CH>()( str1.c_str(), str2.c_str() ) < 0;
        }
    };

    /************************************************************
    ********    functions
    ************************************************************/
    std::string NarrowStringStrict( const std::wstring& str );
    std::wstring WidenStringStrict( const std::string& str );

    /************************************************************
    ********    StrConvertTrait
    ************************************************************/
    template <class SYSTEM_CH, class PARAM_CH> struct StrConvertTrait;

    template <> struct StrConvertTrait<char, char>
    {
        static std::string Convert( const std::string& str )        { return str; }
    };

    template <> struct StrConvertTrait<wchar_t, wchar_t>
    {
        static std::wstring Convert( const std::wstring& str )      { return str; }
    };

    template <> struct StrConvertTrait<char, wchar_t>
    {
        static std::string Convert( const std::wstring& str )       { return NarrowStringStrict( str ); }
    };

    template <> struct StrConvertTrait<wchar_t, char>
    {
        static std::wstring Convert( const std::string& str )       { return WidenStringStrict( str ); }
    };

    /************************************************************/

    inline std_string LocalString( const std::wstring& str )
    {
        return StrConvertTrait<std_char, std::wstring::value_type>::Convert( str );
    }

    inline std_string LocalString( const wchar_t *str )
    {
        return StrConvertTrait<std_char, wchar_t>::Convert( std::wstring( str ) );
    }

    inline std_string LocalString( const std::string& str )
    {
        return StrConvertTrait<std_char, std::string::value_type>::Convert( str );
    }

    inline std_string LocalString( const char *str )
    {
        return StrConvertTrait<std_char, char>::Convert( std::string( str ) );
    }

    inline std::string NarrowString( const std_string& str )
    {
        return StrConvertTrait<std::string::value_type, std_char>::Convert( str );
    }

    inline std::wstring WidenString( const std_string& str )
    {
        return StrConvertTrait<std::wstring::value_type, std_char>::Convert( str );
    }

    int CompareText( const std::string& S1, const std::string& S2 );
    int CompareText( const std::wstring& S1, const std::wstring& S2 );
    std::wstring Trim( const std::wstring& str );
    std::string Trim( const std::string& str );
    std::string StringReplace( const std::string& str, const std::string& old_pattern, const std::string& new_pattern );
    std::wstring StringReplace( const std::wstring& str, const std::wstring& old_pattern, const std::wstring& new_pattern );

    //===============================================================
    //==========    String Functions
    //===============================================================
    std::string  LowerCase( const std::string& src );
    std::wstring LowerCase( const std::wstring& src );
    std::string  UpperCase( const std::string& src );
    std::wstring UpperCase( const std::wstring& src );

}

#endif
