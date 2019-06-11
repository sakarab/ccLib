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
#include "cpp_string.h"
#include <cc_array.hpp>
#include <cc_memory.hpp>
#include <boost/smart_ptr.hpp>
#include <Windows.h>

namespace
{
    UINT LCIDToCodePage( DWORD ALcid )
    {
        cclib::array<char, sizeof( DWORD ) * 2>       Buffer;

        std::fill( Buffer.begin(), Buffer.end(), char() );
        if ( GetLocaleInfo( ALcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER,
                            reinterpret_cast<LPWSTR>(Buffer.data()), cclib::size_cast<unsigned int>(Buffer.size()) ) )
            return *reinterpret_cast<UINT *>(&Buffer.front());
        return CP_ACP;
    }

    /************************************************************
    ********    DefaultUserCodePage
    ************************************************************/
    class DefaultUserCodePage
    {
    private:
        static UINT InitValue()
        {
            DWORD   version = GetVersion();

            // High bit is set for Win95/98/ME
            if ( (version & 0x80000000) != 0x80000000 )
            {
                if ( LOBYTE( version ) > 4 )
                    return CP_THREAD_ACP;                       // 3 - Use CP_THREAD_ACP with Win2K/XP
                else
                    return LCIDToCodePage( GetThreadLocale() ); // Use thread's current locale with NT4
            }
            else
                return LCIDToCodePage( GetThreadLocale() );     // Convert thread's current locale with Win95/98/ME
        }

        DefaultUserCodePage();
    public:
        static UINT Value()
        {
            static UINT the_value = InitValue();

            return the_value;
        }
    };

    /************************************************************
    ********    String Functions (Char conversions)
    ***********************************************************/
    int CharFromWChar( const std::wstring& str, std::string& result_str,
                       char *buffer, std::size_t buffer_chars )
    {
        BOOL            used_default_char = FALSE;
        unsigned int    buffer_size = cclib::size_cast<unsigned int>(buffer_chars);
        int             result = WideCharToMultiByte( DefaultUserCodePage::Value(), 0,
                                                      str.c_str(), cclib::size_cast<unsigned int>(str.length()),
                                                      buffer, buffer_size, 0, &used_default_char );

        //DUMPMSG1_IF( (result == 0), "CharFromWChar failed: %1%", GetLastError() );
        //DUMPMSG_IF( (used_default_char), "CharFromWChar failed on some characters" );
        if ( result > 0 && buffer_size > 0 )
            result_str = std::string( buffer, buffer + result );
        return result;
    }

    int WCharFromChar( const std::string& str, std::wstring& result_str,
                       wchar_t *buffer, std::size_t buffer_chars )
    {
        int     result = MultiByteToWideChar( DefaultUserCodePage::Value(), 0,
                                              str.c_str(), cclib::size_cast<unsigned int>(str.length()),
                                              buffer, cclib::size_cast<unsigned int>(buffer_chars) );

        if ( result >= 0 )
            result_str = std::wstring( buffer, buffer + result );
        return result;
    }

    /************************************************************
    ********    String Functions (Comparison)
    ***********************************************************/
    template<class CH, class COMPARATOR>
    int CompareTextIN( CH *S1, unsigned int S1_len, CH *S2, unsigned int S2_len, COMPARATOR compare )
    {
        return compare( LOCALE_USER_DEFAULT, NORM_IGNORECASE, S1, S1_len, S2, S2_len ) - 2;
    }

    /************************************************************
    ********    String Functions (Trim)
    ***********************************************************/
    template <class CHAR_T>
    std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >
    Trim_in( const std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >& str )
    {
        typedef std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >   str_type;

        typename str_type::size_type    len = str.length();
        typename str_type::size_type    low = 0;

        while ( low < len && str[low] <= cclib::CharConstant<CHAR_T>::sp )
            ++low;
        while ( low < len && str[len-1] <= cclib::CharConstant<CHAR_T>::sp )
            --len;
        return str.substr( low, len - low );
    }

    template <class CHAR_T>
    std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >
    TrimRight_in( const std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >& str )
    {
        typedef std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >   str_type;

        typename str_type::size_type    len = str.length();

        while ( 0 < len && str[len-1] <= cclib::CharConstant<CHAR_T>::sp )
            --len;
        return str.substr( 0, len );
    }

    template <class CHAR_T> void
    InplaceTrimRight_in( std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >& str )
    {
        typedef std::basic_string< CHAR_T, std::char_traits<CHAR_T>, std::allocator<CHAR_T> >   str_type;

        typename str_type::size_type    len = str.length();

        while ( 0 < len && str[len-1] <= cclib::CharConstant<CHAR_T>::sp )
            --len;
        str.resize( len );
    }

    /************************************************************
    ********    String Functions (StuffString)
    ***********************************************************/
    template <class STR_TYPE>
    STR_TYPE StuffString_in( const STR_TYPE& text, typename STR_TYPE::size_type start,
                             typename STR_TYPE::size_type length, const STR_TYPE& sub_text )
    {
        return text.substr( 0, start ) + sub_text + text.substr( start + length );
    }

    /************************************************************
    ********    String Functions (StringReplace)
    ***********************************************************/
    template <class STR_TYPE>
    STR_TYPE StringReplace_in( const STR_TYPE& str, const STR_TYPE& old_pattern, const STR_TYPE& new_pattern )
    {
        STR_TYPE                        result = str;
        typename STR_TYPE::size_type    off = 0;

        do
        {
            off = result.find( old_pattern, off );
            if ( off != STR_TYPE::npos )
            {
                result = StuffString_in( result, off, old_pattern.length(), new_pattern );
                off += new_pattern.length();
            }
        } while ( off != STR_TYPE::npos );
        return result;
    }

    /************************************************************
    ********    String Functions (Lower/Upper)
    ***********************************************************/
    template<class CH, class CONVERTER>
    std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >
        CharConvert( const std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >& src, CONVERTER convert )
    {
        typedef std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >   str_type;

        if ( src.empty() )
            return str_type();

        std::vector<str_type::value_type>       vec_result;

        std::copy( src.begin(), src.end(), std::back_inserter( vec_result ) );
        vec_result.push_back( 0 );
        convert( &vec_result.front() );
        return str_type( &vec_result.front(), vec_result.size() );
    }
}
// namespace

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
    
    std::wstring WidenStringStrict( const std::string& str )
    {
        std::wstring     result;

        if ( str.empty() )
            return result;

        cclib::array<wchar_t, 2048>     buffer;
        std::size_t                     hype_len = str.length() * 2 + 1;    // overallocate

        if ( hype_len < buffer.size() )
        {
            int     dest_len = WCharFromChar( str, result, buffer.data(), buffer.size() );

            if ( dest_len >= 0 )
                return std::wstring( buffer.data(), buffer.data() + dest_len );
        }

        boost::scoped_array<wchar_t>    bbuff( new wchar_t[hype_len] );
        int                             dest_len = WCharFromChar( str, result, bbuff.get(), hype_len );

        if ( dest_len >= 0 )
            return std::wstring( bbuff.get(), bbuff.get() + dest_len );
        return std::wstring();
    }

    int CompareText( const std::string& S1, const std::string& S2 )                                     { return CompareTextIN( S1.c_str(), S1.length(), S2.c_str(), S2.length(), CompareStringA ); }
    int CompareText( const std::wstring& S1, const std::wstring& S2 )                                   { return CompareTextIN( S1.c_str(), S1.length(), S2.c_str(), S2.length(), CompareStringW ); }
    int CompareText( const char *S1, unsigned int S1_len, const char *S2, unsigned int S2_len )         { return CompareTextIN( S1, S1_len, S2, S2_len, CompareStringA ); }
    int CompareText( const wchar_t *S1, unsigned int S1_len, const wchar_t *S2, unsigned int S2_len )   { return CompareTextIN( S1, S1_len, S2, S2_len, CompareStringW ); }

    std::wstring Trim( const std::wstring& str )        { return Trim_in( str ); }
    std::string Trim( const std::string& str )          { return Trim_in( str ); }
    std::wstring TrimRight( const std::wstring& str )   { return TrimRight_in( str ); }
    std::string TrimRight( const std::string& str )     { return TrimRight_in( str ); }
    void InplaceTrimRight( std::wstring& str )          { InplaceTrimRight_in( str ); }
    void InplaceTrimRight( std::string& str )           { InplaceTrimRight_in( str ); }

    std::string StringReplace( const std::string& str, const std::string& old_pattern, const std::string& new_pattern )
    {
        return StringReplace_in( str, old_pattern, new_pattern );
    }

    std::wstring StringReplace( const std::wstring& str, const std::wstring& old_pattern, const std::wstring& new_pattern )
    {
        return StringReplace_in( str, old_pattern, new_pattern );
    }

    /************************************************************
    ********    String Functions (Lower/Upper)
    ***********************************************************/
    std::string  LowerCase( const std::string& src )        { return CharConvert( src, CharLowerA ); }
    std::wstring LowerCase( const std::wstring& src )       { return CharConvert( src, CharLowerW ); }
    std::string  UpperCase( const std::string& src )        { return CharConvert( src, CharUpperA ); }
    std::wstring UpperCase( const std::wstring& src )       { return CharConvert( src, CharUpperW ); }
}
