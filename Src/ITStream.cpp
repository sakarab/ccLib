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
