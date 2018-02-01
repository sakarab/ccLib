//---------------------------------------------------------------------------
#ifndef CStreamsH
#define CStreamsH
//---------------------------------------------------------------------------
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
	CStream&	(*__pf)( CStream&, T );
	T			__manarg;
	CManip( CStream& (*pf)( CStream&, T ), T manarg ) : __pf(pf), __manarg(manarg)
	{ ; }
};

class CManipInt
{
	typedef CStream& (*ManipInt)( CStream& st, unsigned int a );
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
	unsigned int CalcShiftSize( CStream& st );
	long InternalCopyFrom( CStream& source, long count );
	CStream( const CStream& );
	CStream& operator=( const CStream& );
protected:
	virtual long GetPosition();
	virtual void SetPosition( long pos );
	virtual long GetSize();
	virtual void SetSize( long /* new_size */ );
public:
	CStream() : FWidth( 0 )									    {} // empty
	virtual ~CStream()											{} // empty
	virtual long Read( void *buffer, long count ) = 0;
	virtual long Write( const void *buffer, long count ) = 0;
	virtual long Seek( long offset, smode origin ) = 0;
	CStream& ReadBuffer( void *buffer, long count );
	CStream& WriteBuffer( const void *buffer, long count );
	long CopyFrom( CStream& source, long count );

	CStream& operator << ( const char *a );
	CStream& operator << ( CStream& st );

	CStream& operator >> ( const char *a );
	CStream& operator >> ( CStream& st );
	CStream& operator >> ( const CManipInt& f );

	static CManipInt setw( unsigned int width );
	static CStream& swidth( CStream& st, unsigned int a );

	__property long Position = { read=GetPosition, write=SetPosition };
	__property long Size = { read=GetSize, write=SetSize };
};

CStream& operator << ( CStream& st, const bool a );
CStream& operator << ( CStream& st, const char a );
CStream& operator << ( CStream& st, const int a );
CStream& operator << ( CStream& st, const unsigned int a );
CStream& operator << ( CStream& st, const long a );
CStream& operator << ( CStream& st, const unsigned long a );
CStream& operator << ( CStream& st, const std::string& a );

CStream& operator >> ( CStream& st, bool a );
CStream& operator >> ( CStream& st, char& a );
CStream& operator >> ( CStream& st, int& a );
CStream& operator >> ( CStream& st, unsigned int& a );
CStream& operator >> ( CStream& st, long& a );
CStream& operator >> ( CStream& st, unsigned long& a );
CStream& operator >> ( CStream& st, std::string& a );

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
	HANDLE GetHandle()												{ return ( FHandle ); }
	NO_COPY_CTOR(CFileStream);
	NO_COPY_OPER(CFileStream);
protected:
	virtual void SetSize( long new_size );
public:
	CFileStream()
		: CStream(), FHandle(INVALID_HANDLE_VALUE), FName()					{ /* empty */ }
	CFileStream( const char *FileName, Word Mode );
	~CFileStream();
	void Open( const char *FileName, Word Mode );
	void Close();
	bool IsOpen()													{ return ( FHandle != INVALID_HANDLE_VALUE ); }
	virtual long Read( void *buffer, long count );
	virtual long Write( const void *buffer, long count );
	virtual long Seek( long offset, smode origin );
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
	long InternalGetSize()										{ return ( FEnd - FMem ); }
	long InternalGetPosition()									{ return ( FPtr - FMem ); }
	void CheckedAdvance( char *new_ptr )
	{
		if ( new_ptr <= FEnd && new_ptr >= FMem )
			FPtr = new_ptr;
	}
protected:
	virtual long GetPosition();
	virtual long GetSize();
public:
	CCustomMemoryStream( void *ptr, int size );
	virtual long Read( void *buffer, long count );
	virtual long Seek( long offset, smode origin );
	virtual long Write( const void *buffer, long count );
	void LoadFromStream( CStream& stream );
	void LoadFromFile( const char *& file_name );
	void SaveToStream( CStream& stream );
	void SaveToFile( const char *& file_name );
};

class CMemoryStream : public CStream
{
private:
	enum { MemoryDelta = 0x2000 };		// Must be a power of 2
	void	*FMemory;
	int		FSize;
	int		FPosition;
	int		FCapacity;
	void SetCapacity( int new_capacity );
	void * GetMemory()											{ return ( FMemory ); }
	int GetCapacity()											{ return ( FCapacity ); }
	NO_COPY_CTOR(CMemoryStream);
	NO_COPY_OPER(CMemoryStream);
protected:
	virtual void SetSize( long new_size );
	void * Realloc( int new_capacity );
	void SetPointer( void *ptr, int size );
public:
	CMemoryStream();
	~CMemoryStream();
	virtual long Read( void *buffer, long count );
	virtual long Seek( long offset, smode origin );
	virtual long Write( const void *buffer, long count );
	void Clear();
	void LoadFromStream( CStream& stream );
	void LoadFromFile( const char *& file_name );
	void SaveToStream( CStream& stream );
	void SaveToFile( const char *& file_name );
	__property void *Memory = { read=GetMemory };
};
//#pragma option pop

}; // end namespace cc
//---------------------------------------------------------------------------
#endif

