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
#include <vector>

#include "ITStream.h"
#include "cc_memory.hpp"

namespace cclib
{
    //===========================================================
    //======    ITStream
    //===========================================================
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
        std::vector<char>   buffer( cclib::size_cast<size_t>(buff_size) );

        while ( count != 0 )
        {
            size_type   n = count > buff_size ? buff_size : count;

            src.ReadBuffer( &buffer.front(), size_cast<size_t>(n) );
            WriteBuffer( &buffer.front(), size_cast<size_t>(n) );
            count -= n;
        }
        return result;
    }

    void ITStream::ReadBuffer( void *mem, size_t count )
    {
        if ( count != 0 && ReadInternal( mem, count ) != count )
            Raise_LastError();
        //{
        //    throw std::runtime_error( boost::str( boost::format( "%1% Error: 0x%2%, 0x%3%, %4%" )
        //                                          % "Read"
        //                                          % boost::io::group( std::uppercase, std::hex, TCommStream_ErrValue() )
        //                                          % boost::io::group( std::uppercase, std::hex, TCommStream_LastError() )
        //                                          % NarrowString( SysErrorMessage( TCommStream_LastError() ) ) ),
        //                              __LINE__, __FILE__ );
        //}
    }

    void ITStream::WriteBuffer( const void *mem, size_t count )
    {
        if ( count != 0 && WriteInternal( mem, count ) != count )
            Raise_LastError();
            //throw BaseException( boost::str( boost::format( "%1% Error: 0x%2%, 0x%3%, %4%" )
            //                                 % "Write"
            //                                 % boost::io::group( std::uppercase, std::hex, TCommStream_ErrValue() )
            //                                 % boost::io::group( std::uppercase, std::hex, TCommStream_LastError() )
            //                                 % NarrowString( SysErrorMessage( TCommStream_LastError() ) ) ),
            //                     __LINE__, __FILE__ );
    }
}
//namespace cclib
