//---------------------------------------------------------------------------
#ifndef ZLibStreamH
#define ZLibStreamH
//---------------------------------------------------------------------------
#include <exception>
#include "CStreams.h"
#include "zlib.h"
#include "ssport.h"

namespace cc
{

class CZStreamError : protected std::exception
{
public:
    FASTCALL CZStreamError( const char *msg );
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
    FASTCALL CZCustomStream( CStream& strm );
};

class CZCompressionStream : public CZCustomStream
{
public:
    FASTCALL CZCompressionStream( CStream& strm, CZCompressionLevel compression_level );
    virtual FASTCALL ~CZCompressionStream();
	virtual long FASTCALL Read( void *buffer, long count );
	virtual long FASTCALL Write( const void *buffer, long count );
	virtual long FASTCALL Seek( long offset, smode origin );
};

class CZDecompressionStream : public CZCustomStream
{
public:
    FASTCALL CZDecompressionStream( CStream& strm );
    virtual FASTCALL ~CZDecompressionStream();
	virtual long FASTCALL Read( void *buffer, long count );
	virtual long FASTCALL Write( const void *buffer, long count );
	virtual long FASTCALL Seek( long offset, smode origin );
};

void CompressBuf( const void *InBuf, int InBytes, void *& OutBuf, int& OutBytes );
void DecompressBuf( const void *InBuf, int InBytes, int OutEstimate, void *& OutBuf, int& OutBytes );

};
//---------------------------------------------------------------------------
#endif
