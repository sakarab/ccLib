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
    typedef cclib::array<char, 65>      base64charset;

    base64charset Base64Set = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '=' };
    base64charset Url64Set  = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_', '~' };

    std::vector<char> Any64Encode( const unsigned char *source, int sourceLen, const base64charset& charset )
    {
        unsigned char       buf[4];
        unsigned int        avail, j;
        std::vector<char>   result;

        avail = 3;
        while ( sourceLen > 0 ) {
            if ( sourceLen > 3 )
                avail = 3;
            else
                avail = sourceLen;
            sourceLen -= avail;
            *reinterpret_cast<unsigned int *>(buf) = 0;
            j = 2;
            for ( unsigned int i = 0 ; i < avail ; ++i ) {
                buf[j] = *source;
                ++source;
                --j;
            }
            for ( unsigned int i = 0 ; i <= avail ; ++i ) {
                j = (buf[2] & 0xFC) >> 2;
                *reinterpret_cast<unsigned int *>(buf) = *reinterpret_cast<unsigned int *>(buf) << 6;
                result.push_back( charset[j] );
            }
        }
        result.insert( result.end(), 3-avail, charset[65] );
        return result;
    }
}

namespace cclib
{
    std::string Base64Encode( const std::vector<unsigned char>& src )
    {
        if ( src.empty() )
            return std::string();

        std::vector<char>   encoded = Any64Encode( &src.front(), src.size(), Base64Set );

        return std::string( encoded.begin(), encoded.end() );
    }

    std::vector<unsigned char> Base64Decode( const std::string& src )
    {
        return std::vector<unsigned char>();
    }
}
