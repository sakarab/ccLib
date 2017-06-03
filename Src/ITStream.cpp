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
#include "ITStream.h"
#include <vector>
#include "smException.h"

namespace cclib
{

/************************************************************
********    ITStream
************************************************************/
ITStream::size_type ITStream::CopyFrom( ITStream& src, size_type count )
{
    const int   MaxBufSize = 0xF000;

    if ( count == 0 )
    {
        src.set_position( 0 );
        count = src.get_size();
    }

    size_type           result = count;
    size_type           buff_size = count > MaxBufSize ? MaxBufSize : count;
    std::vector<char>   buffer( buff_size );

    while ( count != 0 )
    {
        size_type   n = count > buff_size ? buff_size : count;

        src.ReadBuffer( &buffer.front(), n );
        WriteBuffer( &buffer.front(), n );
        count -= n;
    }
    return result;
}

void ITStream::ReadBuffer( void *mem, int count )
{
    if ( count != 0 && ReadInternal( mem, count ) != count )
        throw BaseException( "Read Error." );
}

void ITStream::WriteBuffer( const void *mem, int count )
{
    if ( count != 0 && WriteInternal( mem, count ) != count )
        throw BaseException( "Write Error" );
}

}
//namespace cclib
