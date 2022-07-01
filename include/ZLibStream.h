//---------------------------------------------------------------------------
#ifndef ZLibStreamH
#define ZLibStreamH
//---------------------------------------------------------------------------
#include <exception>
#include "CStreams.h"
#include "zlib.h"

namespace cc
{

class CZStreamError : protected std::exception
{
public:
    CZStreamError( const char *msg );
    virtual const char* what() const throw();
};

enum CZCompressionLevel { clNone, clFastest, clDefault, clMax };

class CZCustomStream : public CStream
{
protected:
    CStream&        FStrm;
    int             FStrmPos;
    z_stream        FZRec;
    unsigned char   FBuffer[2];
public:
    CZCustomStream( CStream& strm );
};

class CZCompressionStream : public CZCustomStream
{
public:
    CZCompressionStream( CStream& strm, CZCompressionLevel compression_level );
    virtual ~CZCompressionStream();
	virtual long Read( void *buffer, long count );
	virtual long Write( const void *buffer, long count );
	virtual long Seek( long offset, smode origin );
};

class CZDecompressionStream : public CZCustomStream
{
public:
    CZDecompressionStream( CStream& strm );
    virtual ~CZDecompressionStream();
	virtual long Read( void *buffer, long count );
	virtual long Write( const void *buffer, long count );
	virtual long Seek( long offset, smode origin );
};

void CompressBuf( const void *InBuf, int InBytes, void *& OutBuf, int& OutBytes );
void DecompressBuf( const void *InBuf, int InBytes, int OutEstimate, void *& OutBuf, int& OutBytes );

};
//---------------------------------------------------------------------------
#endif
