#include "StringUtils.h"
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

namespace // anonymous
{
    UINT LCIDToCodePage( DWORD ALcid )
    {
        boost::array<WCHAR,7>   Buffer;

        std::fill( Buffer.begin(), Buffer.end(), WCHAR() );
        GetLocaleInfo( ALcid, LOCALE_IDEFAULTANSICODEPAGE, Buffer.data(), static_cast<unsigned int>(Buffer.size()) );

        UINT    result;

        if ( ! dvc::StrToUInt( Buffer.data(), result ) )
            result = CP_ACP;
        return result;
    }
/************************************************************
********    DefaultUserCodePage
************************************************************/
    class DefaultUserCodePage
    {
    private:
        static UINT InitValue();
        DefaultUserCodePage();
    public:
        static UINT Value();
    };

    UINT DefaultUserCodePage::InitValue()
    {
        DWORD   version = GetVersion();

        // High bit is set for Win95/98/ME
        if ( (version & 0x80000000) != 0x80000000 )
        {
            if ( LOBYTE(version) > 4 )
                return CP_THREAD_ACP;                       // 3 - Use CP_THREAD_ACP with Win2K/XP
            else
                return LCIDToCodePage( GetThreadLocale() ); // Use thread's current locale with NT4
        }
        else
            return LCIDToCodePage( GetThreadLocale() );     // Convert thread's current locale with Win95/98/ME
    }

    UINT DefaultUserCodePage::Value()
    {
        static UINT the_value = InitValue();

        return the_value;
    }
//***********************************************************
    int CharFromWChar( const std::wstring& str, char *buffer, std::size_t buffer_size )
    {
        return WideCharToMultiByte( DefaultUserCodePage::Value(), 0,
                                    str.c_str(), static_cast<unsigned int>(str.length()),
                                    buffer, static_cast<int>(buffer_size), nullptr, nullptr );
    }

    std::string NarrowString_make( char *src, int len )
    {
        return std::string( src, src + len );
    }

    std::string NarrowString_in( const std::wstring& str )
    {
        std::string     result;

        if ( str.empty() )
            return result;

        std::size_t         hype_len = str.length() * 3 + 1;        // overallocate

        if ( hype_len < 4096 )
        {
            boost::array<char,4096>     buffer;
            int                         dest_len = CharFromWChar( str, buffer.data(), buffer.size() );

            if ( dest_len > 0 )
                result = NarrowString_make( buffer.data(), dest_len );
        }
        else
        {
            boost::scoped_array<char>   bbuff( new char[hype_len] );
            int                         dest_len = CharFromWChar( str, bbuff.get(), hype_len );

            if ( dest_len > 0 )
                result = NarrowString_make( bbuff.get(), dest_len );
        }
        return result;
    }
//***********************************************************
    int WCharFromChar( const std::string& str, wchar_t *buffer, std::size_t buffer_chars )
    {
        return MultiByteToWideChar( DefaultUserCodePage::Value(), 0,
                                    str.c_str(), static_cast<unsigned int>(str.length()),
                                    buffer, static_cast<unsigned int>(buffer_chars) );
    }

    std::wstring WideString_make( wchar_t *src, int len )
    {
        return std::wstring( src, src + len );
    }

    std::wstring WidenString_in( const std::string& str )
    {
        std::wstring        result;

        if ( str.empty() )
            return result;

        std::size_t         hype_len = str.length() * 2 + 1;    // overallocate

        if ( hype_len < 2048 )
        {
            boost::array<wchar_t,2048>      buffer;
            int                             dest_len = WCharFromChar( str, buffer.data(), buffer.size() );

            if ( dest_len > 0 )
                result = WideString_make( buffer.data(), dest_len );
        }
        else
        {
            boost::scoped_array<wchar_t>    bbuff( new wchar_t[hype_len] );
            int                             dest_len = WCharFromChar( str, bbuff.get(), hype_len );

            if ( dest_len > 0 )
                return WideString_make( bbuff.get(), dest_len );
        }
        return std::wstring();
    }

//***********************************************************
}
// namespace // anonymous

namespace dvc
{
    std::string NarrowString( const std::wstring& str )
    {
        return NarrowString_in( str );
    }

    std::wstring WidenString( const std::string& str )
    {
        return WidenString_in( str );
    }

    bool StrToUInt( const std::wstring& str, UINT& int_result )
    {
        bool    result;

        try
        {
            int_result = boost::lexical_cast<UINT>(str);
            result = true;
        }
        catch ( const boost::bad_lexical_cast& )
        {
            result = false;
        }
        return result;
    }

    std::wstring BinToHex( const char *bytes, unsigned int length )
    {
        static const wchar_t   hex_map[] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
                                             L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' };

        std::vector<wchar_t>   res( 2 * length );

        for ( unsigned int n = 0 ; n < length ; ++n )
        {
            res[2*n]   = hex_map[static_cast<unsigned char>(bytes[n]) >> 4];
            res[2*n+1] = hex_map[static_cast<unsigned char>(bytes[n]) & 0xF];
        }
        return std::wstring( &res.front(), res.size() );
    }
}
// namespace dvc
