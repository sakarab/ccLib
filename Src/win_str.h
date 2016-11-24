#pragma once

#ifndef CCLIB_WIN_STR_H
#define CCLIB_WIN_STR_H

#include <Windows.h>
#include "cpp_string.h"

namespace ccwin
{

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
    //************************************************************/

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

}

#endif
