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

#if ! defined (CC_BOM_UTILS_H)
#define CC_BOM_UTILS_H

#include <cpp_string.h>

/**************************************************************************
********    BOM
**************************************************************************/
struct BOM
{
private:
    // noncopyable
    BOM( const BOM& src );
    BOM& operator=( const BOM& src );
    // nonconstructible
    BOM();
private:
    static std_char const *Names[];
public:
    enum type { no_bom, utf8, utf16le, utf16be, utf32le, utf32be };

    static std_string Name( type bom );
    static type Value( std_string name );
};

/**************************************************************************
********    Free functions
**************************************************************************/
BOM::type GetBom( const std_string& file_name );
BOM::type GetBom( const std_char *file_name );

#endif
