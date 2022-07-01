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

#ifndef CCLIB_CPP_CHAR_UTILS_H
#define CCLIB_CPP_CHAR_UTILS_H

#include <limits>
#include <vector>
#include <string>
#include <cpp_string.h>

namespace cclib
{
    template <class T> T rotate_left( T value, unsigned int count )
    {
        const unsigned int      mask = std::numeric_limits<T>::digits - 1;  // assumes width is a power of 2.

        return (value << count) | (value >> ((~count + 1) & mask));
    }

    template <class T> T rotate_right( T value, unsigned int count )
    {
        const unsigned int      mask = std::numeric_limits<T>::digits - 1;  // assumes width is a power of 2.

        return (value >> count) | (value << ((~count + 1) & mask));
    }

    std::string Base64Encode( const std::vector<unsigned char>& src );
    std::string Base64Encode( const unsigned char * const src, size_t src_len );
    std::string Base64Encode( const string_view& src );
    std::vector<unsigned char> Base64Decode( const std::string& src );
    std::vector<unsigned char> Base64Decode( const char * const src, size_t src_len );
    std::vector<unsigned char> Base64Decode( const string_view& src );
}

#endif
