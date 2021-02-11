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
#include <cppCharUtils.h>
#include <cc_array.hpp>

namespace
{
    typedef cclib::array<char, 65>              base64charset;
    typedef cclib::array<unsigned char, 256>    reverse64charset;

    base64charset       Base64Set = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '='
    };
    //base64charset       Url64Set  = {
    //    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    //    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    //    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_', '~'
    //};

    reverse64charset    ReverseSet = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    void Any64Encode_3( const unsigned char *source_end, size_t source_len_max3, const base64charset& charset, std::vector<char>& result )
    {
        unsigned int        bytes3 = 0;

        for ( size_t n = 0 ; n < source_len_max3 - 1 ; ++n )
        {
            bytes3 |= *(--source_end);
            bytes3 <<= 8;
        }
        bytes3 |= *(--source_end);

        for ( size_t n = 0, eend = source_len_max3 * 8 ; n < eend ; n += 6 )
        {
            result.push_back( charset[bytes3 & 0x3F] );
            bytes3 >>= 6;
        }
    }

    std::vector<char> Any64Encode( const unsigned char *source, size_t source_len, const base64charset& charset )
    {
        std::vector<char>   result;
        size_t              source_len_max3 = 3;

        while ( source_len != 0 )
        {
            source_len_max3 = std::min( static_cast<size_t>(3), source_len );
            source += source_len_max3;
            Any64Encode_3( source, source_len_max3, charset, result );
            source_len -= source_len_max3;
        }
        result.insert( result.end(), 3-source_len_max3, charset[64] );
        return result;
    }

    // non portable
    std::vector<char> Any64Encode_NP( const unsigned char *source, size_t source_len, const base64charset& charset )
    {
        unsigned char       buf[4];
        size_t              avail;
        std::vector<char>   result;

        avail = 3;
        while ( source_len > 0 )
        {
            if ( source_len > 3 )
                avail = 3;
            else
                avail = source_len;
            source_len -= avail;
            *reinterpret_cast<unsigned int *>(buf) = 0;
            for ( unsigned int i = 0, j = 2 ; i < avail ; ++i, --j )
                buf[j] = *source++;
            for ( unsigned int i = 0 ; i <= avail ; ++i )
            {
                result.push_back( charset[((buf[2] & 0xFC) >> 2)] );
                *reinterpret_cast<unsigned int *>(buf) = *reinterpret_cast<unsigned int *>(buf) << 6;
            }
        }
        result.insert( result.end(), 3-avail, charset[64] );
        return result;
    }

    const reverse64charset& ReverseCharset_Create( const base64charset& charset )
    {
        reverse64charset&       result = ReverseSet;

        if ( result[charset[0]] == 0xFF )
            for ( size_t n = 0, eend = charset.size() ; n < eend ; ++n )
                result[charset[n]] = static_cast<unsigned char>(n);
        return result;
    }

    std::vector<unsigned char> Any64Decode_NP( const char *source, size_t source_len, const reverse64charset& reverse_charset )
    {
        unsigned char                   buf[4];
        std::vector<unsigned char>      result;

        if ( source_len % 4 != 0 )
            throw std::runtime_error( "not multiple of 4-chars" );

        while ( source_len > 0 )
        {
            int     avail = 0;

            *reinterpret_cast<unsigned int *>(buf) = 0;
            for ( int i = 0 ; i < 4 ; ++i )
            {
                --source_len;

                int     ch = reverse_charset[*source++];

                if ( ch == 0xFF )
                    throw std::runtime_error( "illegal character" );
                if ( ch == 64 )
                    ch = 0;
                else
                    avail = i;
                *reinterpret_cast<unsigned int *>(buf) = (*reinterpret_cast<unsigned int *>(buf) << 6) | ch;
            }
            for ( int i = 2 ; i >= 3-avail ; --i )
                result.push_back( buf[i] );
        }
        return result;
    }
}

namespace cclib
{
    std::string Base64Encode( const std::vector<unsigned char>& src )
    {
        if ( src.empty() )
            return std::string();

        std::vector<char>   encoded = Any64Encode_NP( &src.front(), src.size(), Base64Set );

        return std::string( encoded.begin(), encoded.end() );
    }

    std::string Base64Encode( const unsigned char * const src, size_t src_len )
    {
        if ( !src || !src_len )
            return std::string();

        std::vector<char>   encoded = Any64Encode_NP( src, src_len, Base64Set );

        return std::string( encoded.begin(), encoded.end() );
    }

    std::string Base64Encode( const string_view& src )
    {
        return Base64Encode( reinterpret_cast<const unsigned char *>(src.data()), src.size() );
    }

    std::vector<unsigned char> Base64Decode( const std::string& src )
    {
        if ( src.empty() )
            return std::vector<unsigned char>();
        return Any64Decode_NP( src.c_str(), src.length(), ReverseCharset_Create( Base64Set ) );
    }

    std::vector<unsigned char> Base64Decode( const char * const src, size_t src_len )
    {
        if ( !src || !src_len )
            return std::vector<unsigned char>();
        return std::vector<unsigned char>();
    }

    std::vector<unsigned char> Base64Decode( const string_view& src )
    {
        return Base64Decode( src.data(), src.size() );
    }
}
