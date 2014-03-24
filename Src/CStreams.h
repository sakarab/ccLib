//---------------------------------------------------------------------------
#ifndef CStreamsH
#define CStreamsH
//---------------------------------------------------------------------------
#include "ssport.h"
#include <string>
#include <SysUtils.hpp>

namespace cc
{

class CStream;

template<class T> class CManip
{
private:
protected:
public:
	CStream&	(FASTCALL *__pf)( CStream&, T );
	T			__manarg;
	FASTCALL CManip( CStream& (*pf)( CStream&, T ), T manarg ) : __pf(pf), __manarg(manarg)
	{ ; }
};

class CManipInt
{
	typedef CStream& (FASTCALL *ManipInt)( CStream& st, unsigned int a );
public:
	unsigned int	__arg;
	ManipInt		__pf;
	CManipInt( ManipInt f, unsigned int arg ) : __arg(arg), __pf(f) {}
};

//#pragma option push -vi
class CStream
{
public:
	typedef enum { po_begin = 0, po_current = 1, po_end = 2 } smode;
private:
	enum { MAX_BUF_SIZE = 0x0000F000 };
	unsigned int	FWidth;
	unsigned int FASTCALL CalcShiftSize( CStream& st );
	long FASTCALL InternalCopyFrom( CStream& source, long count );
	NO_COPY_CTOR(CStream);
	NO_COPY_OPER(CStream);
protected:
	virtual long FASTCALL GetPosition();
	virtual void FASTCALL SetPosition( long pos );
	virtual long FASTCALL GetSize();
	virtual void FASTCALL SetSize( long /* new_size */ );
public:
	FASTCALL CStream() : FWidth( 0 )									{ /* empty */ }
	virtual FASTCALL ~CStream()											{ /* empty */ }
	virtual long FASTCALL Read( void *buffer, long count ) = 0;
	virtual long FASTCALL Write( const void *buffer, long count ) = 0;
	virtual long FASTCALL Seek( long offset, smode origin ) = 0;
	CStream& FASTCALL ReadBuffer( void *buffer, long count );
	CStream& FASTCALL WriteBuffer( const void *buffer, long count );
	long FASTCALL CopyFrom( CStream& source, long count );

	CStream& FASTCALL operator << ( const char *a );
	CStream& FASTCALL operator << ( CStream& st );

	CStream& FASTCALL operator >> ( const char *a );
	CStream& FASTCALL operator >> ( CStream& st );
	CStream& FASTCALL operator >> ( const CManipInt& f );

	static CManipInt FASTCALL setw( unsigned int width );
	static CStream& FASTCALL swidth( CStream& st, unsigned int a );

	__property long Position = { read=GetPosition, write=SetPosition };
	__property long Size = { read=GetSize, write=SetSize };
};

CStream& FASTCALL operator << ( CStream& st, const bool a );
CStream& FASTCALL operator << ( CStream& st, const char a );
CStream& FASTCALL operator << ( CStream& st, const int a );
CStream& FASTCALL operator << ( CStream& st, const unsigned int a );
CStream& FASTCALL operator << ( CStream& st, const long a );
CStream& FASTCALL operator << ( CStream& st, const unsigned long a );
CStream& FASTCALL operator << ( CStream& st, const std::string& a );

CStream& FASTCALL operator >> ( CStream& st, bool a );
CStream& FASTCALL operator >> ( CStream& st, char& a );
CStream& FASTCALL operator >> ( CStream& st, int& a );
CStream& FASTCALL operator >> ( CStream& st, unsigned int& a );
CStream& FASTCALL operator >> ( CStream& st, long& a );
CStream& FASTCALL operator >> ( CStream& st, unsigned long& a );
CStream& FASTCALL operator >> ( CStream& st, std::string& a );

const int fm_create = 0xFFFF;
const int fm_read = fmOpenRead;
const int fm_write = fmOpenWrite;
const int fm_read_write = fmOpenReadWrite;
const int fm_s_compat = fmShareCompat;
const int fm_s_exclusive = fmShareExclusive;
const int fm_s_deny_write = fmShareDenyWrite;
const int fm_s_deny_read = fmShareDenyRead;
const int fm_s_deny_none = fmShareDenyNone;

class CFileStream : public CStream
{
private:
	HANDLE          FHandle;
	std::string     FName;
	HANDLE FASTCALL GetHandle()												{ return ( FHandle ); }
	NO_COPY_CTOR(CFileStream);
	NO_COPY_OPER(CFileStream);
protected:
	virtual void FASTCALL SetSize( long new_size );
public:
	FASTCALL CFileStream()
		: CStream(), FHandle(INVALID_HANDLE_VALUE), FName()					{ /* empty */ }
	FASTCALL CFileStream( const char *FileName, Word Mode );
	FASTCALL ~CFileStream();
	void FASTCALL Open( const char *FileName, Word Mode );
	void FASTCALL Close();
	bool FASTCALL IsOpen()													{ return ( FHandle != INVALID_HANDLE_VALUE ); }
	virtual long FASTCALL Read( void *buffer, long count );
	virtual long FASTCALL Write( const void *buffer, long count );
	virtual long FASTCALL Seek( long offset, smode origin );
	__property HANDLE Handle = { read=GetHandle };
};

class CCustomMemoryStream : public CStream
{
private:
	char	*FMem;
	char	*FEnd;
	char	*FPtr;
	NO_COPY_CTOR(CCustomMemoryStream);
	NO_COPY_OPER(CCustomMemoryStream);
	long FASTCALL InternalGetSize()										{ return ( FEnd - FMem ); }
	long FASTCALL InternalGetPosition()									{ return ( FPtr - FMem ); }
	void FASTCALL CheckedAdvance( char *new_ptr )
	{
		if ( new_ptr <= FEnd && new_ptr >= FMem )
			FPtr = new_ptr;
	}
protected:
	virtual long FASTCALL GetPosition();
	virtual long FASTCALL GetSize();
public:
	FASTCALL CCustomMemoryStream( void *ptr, int size );
	virtual long FASTCALL Read( void *buffer, long count );
	virtual long FASTCALL Seek( long offset, smode origin );
	virtual long FASTCALL Write( const void *buffer, long count );
	void FASTCALL LoadFromStream( CStream& stream );
	void FASTCALL LoadFromFile( const char *& file_name );
	void FASTCALL SaveToStream( CStream& stream );
	void FASTCALL SaveToFile( const char *& file_name );
};

class CMemoryStream : public CStream
{
private:
	enum { MemoryDelta = 0x2000 };		// Must be a power of 2
	void	*FMemory;
	int		FSize;
	int		FPosition;
	int		FCapacity;
	void FASTCALL SetCapacity( int new_capacity );
	void * FASTCALL GetMemory()											{ return ( FMemory ); }
	int FASTCALL GetCapacity()											{ return ( FCapacity ); }
	NO_COPY_CTOR(CMemoryStream);
	NO_COPY_OPER(CMemoryStream);
protected:
	virtual void FASTCALL SetSize( long new_size );
	void * FASTCALL Realloc( int new_capacity );
	void FASTCALL SetPointer( void *ptr, int size );
public:
	FASTCALL CMemoryStream();
	FASTCALL ~CMemoryStream();
	virtual long FASTCALL Read( void *buffer, long count );
	virtual long FASTCALL Seek( long offset, smode origin );
	virtual long FASTCALL Write( const void *buffer, long count );
	void FASTCALL Clear();
	void FASTCALL LoadFromStream( CStream& stream );
	void FASTCALL LoadFromFile( const char *& file_name );
	void FASTCALL SaveToStream( CStream& stream );
	void FASTCALL SaveToFile( const char *& file_name );
	__property void *Memory = { read=GetMemory };
};
//#pragma option pop

}; // end namespace cc
//---------------------------------------------------------------------------
#endif

