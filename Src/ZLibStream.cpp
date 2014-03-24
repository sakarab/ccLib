//---------------------------------------------------------------------------
#pragma hdrstop

#include "ZLibStream.h"
//---------------------------------------------------------------------------

namespace cc
{

#pragma argsused
void *zlibAllocMem( void *AppData, unsigned int Items, unsigned int Size )
{
    return ( calloc( Items, Size ) );
}

#pragma argsused
void zlibFreeMem( void *AppData, void *Block )
{
    free( Block );
}

int CCheck( int code )
{
    if ( code < 0 )
        throw CZStreamError( "Compression error." );
    return ( code );
}

int DCheck( int code )
{
    if ( code < 0 )
        throw CZStreamError( "Decompression error." );
    return ( code );
}

void CompressBuf( const void *InBuf, int InBytes, void *& OutBuf, int& OutBytes )
{
    z_stream    strm;
    void        *P;

    memset( &strm, 0, sizeof(strm) );
    strm.zalloc = zlibAllocMem;
    strm.zfree = zlibFreeMem;
    OutBytes = ((InBytes + InBytes / 10 + 12) + 255) & ~255;
    OutBuf = static_cast<unsigned char *>(malloc( OutBytes ));
    try
    {
        strm.next_in = const_cast<unsigned char *>(static_cast<const unsigned char *>(InBuf));
        strm.avail_in = InBytes;
        strm.next_out = static_cast<unsigned char *>(OutBuf);
        strm.avail_out = OutBytes;
        CCheck( z_deflateInit_( &strm, Z_BEST_COMPRESSION, zlib_version, sizeof(strm) ) );
        try
        {
            while ( CCheck( z_deflate( &strm, Z_FINISH ) ) != Z_STREAM_END )
            {
                P = OutBuf;
                OutBytes += 256;
                OutBuf = static_cast<unsigned char *>(realloc( OutBuf, OutBytes ));
                strm.next_out = static_cast<unsigned char *>(OutBuf) + (strm.next_out - static_cast<unsigned char *>(P));
                strm.avail_out = 256;
            }
        }
        __finally
        {
            CCheck( z_deflateEnd( &strm ) );
        }
        OutBuf = static_cast<unsigned char *>(realloc( OutBuf, strm.total_out ));
        OutBytes = strm.total_out;
    }
    catch ( ... )
    {
        free( OutBuf );
        throw;
    }
}

void DecompressBuf( const void *InBuf, int InBytes, int OutEstimate, void *& OutBuf, int& OutBytes )
{
    z_stream    strm;
    void        *P;
    int         BufInc;

    memset( &strm, 0, sizeof(strm) );
    strm.zalloc = zlibAllocMem;
    strm.zfree = zlibFreeMem;
    BufInc = (InBytes + 255) & ~255;
    if ( OutEstimate == 0 )
        OutBytes = BufInc;
    else
        OutBytes = OutEstimate;
    OutBuf = static_cast<unsigned char *>(malloc( OutBytes ));
    try
    {
        strm.next_in = const_cast<unsigned char *>(static_cast<const unsigned char *>(InBuf));
        strm.avail_in = InBytes;
        strm.next_out = static_cast<unsigned char *>(OutBuf);
        strm.avail_out = OutBytes;
        DCheck( z_inflateInit_( &strm, zlib_version, sizeof(strm) ) );
        try
        {
            while ( DCheck( z_inflate( &strm, Z_FINISH ) ) != Z_STREAM_END )
            {
                P = OutBuf;
                OutBytes += BufInc;
                OutBuf = static_cast<unsigned char *>(realloc( OutBuf, OutBytes ));
                strm.next_out = static_cast<unsigned char *>(OutBuf) + (strm.next_out - static_cast<unsigned char *>(P));
                strm.avail_out = BufInc;
            }
        }
        __finally
        {
            DCheck( z_inflateEnd( &strm ) );
        }
        OutBuf = static_cast<unsigned char *>(realloc( OutBuf, strm.total_out ));
        OutBytes = strm.total_out;
    }
    catch ( ... )
    {
        free( OutBuf );
        throw;
    }
}

//---------------------------------------------------------------------------
//--    CZCustomStream
//---------------------------------------------------------------------------
FASTCALL CZCustomStream::CZCustomStream( CStream& strm )
    : FStrm(strm)
{
    FStrmPos = FStrm.Position;
    FZRec.zalloc = zlibAllocMem;
    FZRec.zfree = zlibFreeMem;
}

//---------------------------------------------------------------------------
//--    CZCompressionStream
//---------------------------------------------------------------------------
FASTCALL CZCompressionStream::CZCompressionStream( CStream& strm, CZCompressionLevel compression_level )
    : CZCustomStream(strm)
{
    static  const short Levels[4] = { Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION };

    FZRec.next_out = FBuffer;
    FZRec.avail_out = sizeof(FBuffer);
    CCheck( z_deflateInit_( &FZRec, Levels[compression_level], zlib_version, sizeof(FZRec) ) < 0 );
}

FASTCALL CZCompressionStream::~CZCompressionStream()
{
    FZRec.next_in = 0;
    FZRec.avail_in = 0;
    try
    {
        if ( FStrm.Position != FStrmPos )
            FStrm.Position = FStrmPos;
        while ( CCheck( z_deflate( &FZRec, Z_FINISH ) ) != Z_STREAM_END && FZRec.avail_out == 0 )
        {
            FStrm.WriteBuffer( FBuffer, sizeof(FBuffer) );
            FZRec.next_out = FBuffer;
            FZRec.avail_out = sizeof(FBuffer);
        }
        if ( FZRec.avail_out < sizeof(FBuffer) )
            FStrm.WriteBuffer( FBuffer, sizeof(FBuffer) - FZRec.avail_out );
    }
    catch ( ... )
    {
        z_deflateEnd( &FZRec );
        throw;
    }
    z_deflateEnd( &FZRec );
}

#pragma argsused
long FASTCALL CZCompressionStream::Read( void *buffer, long count )
{
    throw CZStreamError( "Invalid stream operation" );
}

long FASTCALL CZCompressionStream::Write( const void *buffer, long count )
{
    FZRec.next_in = const_cast<unsigned char *>(static_cast<const unsigned char *>(buffer));
    FZRec.avail_in = count;
    if ( FStrm.Position != FStrmPos )
        FStrm.Position = FStrmPos;
    while ( FZRec.avail_in > 0 )
    {
        CCheck( z_deflate( &FZRec, 0 ) );
        if ( FZRec.avail_out == 0 )
        {
            FStrm.WriteBuffer( FBuffer, sizeof(FBuffer) );
            FZRec.next_out = FBuffer;
            FZRec.avail_out = sizeof(FBuffer);
            FStrmPos = FStrm.Position;
        }
    }
    return ( count );
}

long FASTCALL CZCompressionStream::Seek( long offset, smode origin )
{
    if ( offset != 0 || origin != po_current )
        throw CZStreamError( "Invalid stream operation" );
    return ( FZRec.total_in );
}

//---------------------------------------------------------------------------
//--    CZDecompressionStream
//---------------------------------------------------------------------------
FASTCALL CZDecompressionStream::CZDecompressionStream( CStream& strm )
    : CZCustomStream(strm)
{
    FZRec.next_in = FBuffer;
    FZRec.avail_in = 0;
    DCheck( z_inflateInit_( &FZRec, zlib_version, sizeof(FZRec) ) );
}

FASTCALL CZDecompressionStream::~CZDecompressionStream()
{
    z_inflateEnd( &FZRec );
}

long FASTCALL CZDecompressionStream::Read( void *buffer, long count )
{
    FZRec.next_out = const_cast<unsigned char *>(static_cast<const unsigned char *>(buffer));
    FZRec.avail_out = count;
    if ( FStrm.Position != FStrmPos )
        FStrm.Position = FStrmPos;
    while ( FZRec.avail_out > 0 )
    {
        if ( FZRec.avail_in == 0 )
        {
            FZRec.avail_in = FStrm.Read( FBuffer, sizeof(FBuffer) );
            if ( FZRec.avail_in == 0 )
                return ( count - FZRec.avail_out );
            FZRec.next_in = FBuffer;
            FStrmPos = FStrm.Position;
        }
        CCheck( z_inflate( &FZRec, 0 ) );
    }
    return ( count );
}

#pragma argsused
long FASTCALL CZDecompressionStream::Write( const void *buffer, long count )
{
    throw CZStreamError( "Invalid stream operation" );
}

long FASTCALL CZDecompressionStream::Seek( long offset, smode origin )
{
    char    Buf[4095];

    if ( offset == 0 && origin == po_begin )
    {
        DCheck( z_inflateReset( &FZRec ) );
        FZRec.next_in = FBuffer;
        FZRec.avail_in = 0;
        FStrm.Position = 0;
        FStrmPos = 0;
    }
    else if ( (offset >= 0 && origin == po_current) || (offset - FZRec.total_out > 0 && origin == po_begin) )
    {
        if ( origin == po_begin )
            offset -= FZRec.total_out;
        if ( offset > 0 )
        {
            for ( unsigned int i = 1 ; i <= offset / sizeof(Buf) ; ++i )
                ReadBuffer( Buf, sizeof(Buf) );
            ReadBuffer( Buf, offset % sizeof(Buf) );
        }
    }
    else
        throw CZStreamError( "Invalid stream operation" );
    return( FZRec.total_out );
}

//---------------------------------------------------------------------------

}; // end namespace cc

