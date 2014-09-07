#include "StringUtils.h"
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

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
    int CharFromWChar( const std::wstring& str, std::string& result_str,
                       char *buffer, std::size_t buffer_chars )
    {
        BOOL            used_default_char = FALSE;    
        unsigned int    buffer_size = static_cast<unsigned int>(buffer_chars);
        int             result = WideCharToMultiByte( DefaultUserCodePage::Value(), 0,
                                                      str.c_str(), static_cast<unsigned int>(str.length()),
                                                      buffer, buffer_size, 0, &used_default_char );

        if ( result > 0 && buffer_size > 0 )
            result_str = std::string( buffer, buffer + result );
        return result;
    }
//***********************************************************
    std::string NarrowString_in( const std::wstring& str )
    {
        std::string     result;

        if ( str.empty() )
            return result;

        boost::array<char,4096>     buffer;
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
// namespace // anonymous

namespace dvc
{
    std::string NarrowString( const std::wstring& str )
    {
        return NarrowString_in( str );
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
}
// namespace dvc
