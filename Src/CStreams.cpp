//---------------------------------------------------------------------------
#pragma hdrstop

#include "CStreams.h"
#include "ssport.h"
#include <boost/smart_ptr.hpp>
#include <exception>
#include <sstream>
//---------------------------------------------------------------------------

namespace cc
{

template <class T1, class T2> std::string cFormat( const T1& a1, const T2& a2 )
{
    std::ostringstream  sstr;

    sstr << a1 << a2;
    return sstr.str();
}

template <class T1, class T2, class T3> std::string cFormat( const T1& a1, const T2& a2, const T3& a3 )
{
    std::ostringstream  sstr;

    sstr << a1 << a2 << a3;
    return sstr.str();
}

class ccException : public std::exception
{
private:
    std::string     mMessage;
public:
    CDFASTCALL ccException( const std::string& msg ) : mMessage( msg )          { /* empty */ }
    CDFASTCALL ccException( const char *msg ) : mMessage( msg )                 { /* empty */ }
};

//---------------------------------------------------------------------------
//--	ccEReadError
//---------------------------------------------------------------------------
class ccEReadError : public ccException
{
public:
    CDFASTCALL ccEReadError( const std::string& msg ) : ccException( msg )      { /* empty */ }
    CDFASTCALL ccEReadError( const char *msg ) : ccException( msg )             { /* empty */ }
};
//---------------------------------------------------------------------------
//--	ccEWriteError
//---------------------------------------------------------------------------
class ccEWriteError : public ccException
{
public:
    CDFASTCALL ccEWriteError( const std::string& msg ) : ccException( msg )     { /* empty */ }
    CDFASTCALL ccEWriteError( const char *msg ) : ccException( msg )            { /* empty */ }
};
//---------------------------------------------------------------------------
//--	ccEFCreateError
//---------------------------------------------------------------------------
class ccEFCreateError : public ccException
{
public:
    CDFASTCALL ccEFCreateError( const std::string& msg ) : ccException( msg )   { /* empty */ }
    CDFASTCALL ccEFCreateError( const char *msg ) : ccException( msg )          { /* empty */ }
};
//---------------------------------------------------------------------------
//--	ccEFOpenError
//---------------------------------------------------------------------------
class ccEFOpenError : public ccException
{
public:
    CDFASTCALL ccEFOpenError( const std::string& msg ) : ccException( msg )     { /* empty */ }
    CDFASTCALL ccEFOpenError( const char *msg ) : ccException( msg )            { /* empty */ }
};
//---------------------------------------------------------------------------
//--	ccEStreamError
//---------------------------------------------------------------------------
class ccEStreamError : public ccException
{
public:
    CDFASTCALL ccEStreamError( const std::string& msg ) : ccException( msg )    { /* empty */ }
    CDFASTCALL ccEStreamError( const char *msg ) : ccException( msg )           { /* empty */ }
};
//---------------------------------------------------------------------------
//--	CStream
//---------------------------------------------------------------------------
long FASTCALL CStream::GetPosition()
{
	return ( Seek( 0, po_current ) );
}

void FASTCALL CStream::SetPosition( long pos )
{
	Seek( pos, po_begin );
}

long FASTCALL CStream::GetSize()
{
	long	result, pos;

	pos = Seek( 0, po_current );
	result = Seek( 0, po_end );
	Seek( pos, po_begin );
	return ( result );
}

void FASTCALL CStream::SetSize( long /* new_size */ )
{
}

CStream& FASTCALL CStream::ReadBuffer( void *buffer, long count )
{
	if ( count != 0 && Read( buffer, count ) != count )
		throw ccEReadError( "Read Error" );
	return ( *this );
}

CStream& FASTCALL CStream::WriteBuffer( const void *buffer, long count )
{
	if ( count != 0 && Write( buffer, count ) != count )
		throw ccEWriteError( "Write Error" );
	return ( *this );
}

long FASTCALL CStream::InternalCopyFrom( CStream& source, long count )
{
	long    buf_size, n, result = 0;

	if ( count > 0 )
	{
		result = count;
		buf_size = (count > MAX_BUF_SIZE) ? MAX_BUF_SIZE : count;

		boost::scoped_array<char>   buffer( new char[buf_size] );

		while ( count != 0 )
		{
			n = (count > buf_size) ? buf_size : count;
			source.ReadBuffer( buffer.get(), n );
			WriteBuffer( buffer.get(), n );
			count -= n;
		}
	}
	return ( result );
}

long FASTCALL CStream::CopyFrom( CStream& source, long count )
{
	if ( count == 0 )
	{
		source.Position = 0;
		count = source.Size;
	}
	return ( InternalCopyFrom( source, count ) );
}

unsigned int FASTCALL CStream::CalcShiftSize( CStream& st )
{
	unsigned int	result = st.Size - st.Position;

	if ( FWidth != 0 && result > FWidth )
		result = FWidth;
	return ( result );
}

CStream& FASTCALL CStream::operator << ( const char *a )
{
	return ( WriteBuffer( a, strlen(a) ) );
};

CStream& FASTCALL CStream::operator << ( CStream& st )
{
	unsigned int	size;

	if ( (size = CalcShiftSize( st )) > 0 )
		InternalCopyFrom( st, size );
	FWidth = 0;
	return ( *this );
}

CStream& FASTCALL CStream::operator >> ( const char *a )
{
	long			n;
	unsigned int	buf_size, size;
	char			*p = const_cast<char *>(a);

	if ( (size = CalcShiftSize( *this )) > 0 )
	{
		buf_size = (size > MAX_BUF_SIZE) ? MAX_BUF_SIZE : size;

		boost::scoped_array<char>   buffer( new char[buf_size] );

		while ( size != 0 )
		{
			n = (size > buf_size) ? buf_size : size;
			ReadBuffer( buffer.get(), n );
			memcpy( p, buffer.get(), n );
			p += n;
			size -= n;
		}
	}
	FWidth = 0;
	return ( *this );
}

CStream& FASTCALL CStream::operator >> ( CStream& st )
{
	unsigned int	size;

	if ( (size = CalcShiftSize( *this )) > 0 )
		st.InternalCopyFrom( *this, size );
	FWidth = 0;
	return ( *this );
}

//CStream& FASTCALL CStream::operator >> ( const CManipInt& f )
//{
//	f.__pf( *this, f.__arg );
//	return ( *this );
//}

CStream& FASTCALL CStream::swidth( CStream& st, unsigned int a )
{
	st.FWidth = a;
	return ( st );
}

CManipInt FASTCALL CStream::setw( unsigned int width )
{
	return ( CManipInt( CStream::swidth, width ) );
}

CStream& FASTCALL operator << ( CStream& st, const bool a )
{
	return ( st.WriteBuffer( &a, sizeof(bool) ) );
};

CStream& FASTCALL operator << ( CStream& st, const char a )
{
	return ( st.WriteBuffer( &a, sizeof(char) ) );
};

CStream& FASTCALL operator << ( CStream& st, const int a )
{
	return ( st.WriteBuffer( &a, sizeof(int) ) );
};

CStream& FASTCALL operator << ( CStream& st, const unsigned int a )
{
	return ( st.WriteBuffer( &a, sizeof(int) ) );
};

CStream& FASTCALL operator << ( CStream& st, const long a )
{
	return ( st.WriteBuffer( &a, sizeof(long) ) );
};

CStream& FASTCALL operator << ( CStream& st, const unsigned long a )
{
	return ( st.WriteBuffer( &a, sizeof(long) ) );
};

CStream& FASTCALL operator << ( CStream& st, const std::string& a )
{
	int		count = a.size();
	st << count;
	return ( st.WriteBuffer( a.c_str(), count ) );
}

CStream& FASTCALL operator >> ( CStream& st, bool a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(bool) ) );
};

CStream& FASTCALL operator >> ( CStream& st, char& a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(char) ) );
}

CStream& FASTCALL operator >> ( CStream& st, int& a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(int) ) );
}

CStream& FASTCALL operator >> ( CStream& st, unsigned int& a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(int) ) );
}

CStream& FASTCALL operator >> ( CStream& st, long& a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(long) ) );
}

CStream& FASTCALL operator >> ( CStream& st, unsigned long& a )
{
	return ( st.ReadBuffer( (void *)&a, sizeof(long) ) );
}

CStream& FASTCALL operator >> ( CStream& st, std::string& a )
{
	int		count;
	st >> count;
	a.resize( count, 0 );
	return ( st.ReadBuffer( const_cast<char *>(a.c_str()), count ) );
}
//---------------------------------------------------------------------------
//--	CFileStream
//---------------------------------------------------------------------------
FASTCALL CFileStream::CFileStream( const char *FileName, Word Mode )
	 : CStream(), FHandle(NULL), FName(FileName)
{
	Open( FileName, Mode );
}

FASTCALL CFileStream::~CFileStream()
{
	Close();
}

void FASTCALL CFileStream::Open( const char *FileName, Word Mode )
{
	if ( Mode == fm_create )
	{
		if ( (int)((FHandle = (HANDLE)FileCreate( FileName ))) < 0 )
			throw ccEFCreateError( cFormat( "Unable to create file : ", FileName ) );
	}
	else
	{
		if ( (int)((FHandle = (HANDLE)FileOpen( FileName, Mode ))) < 0 )
			throw ccEFOpenError( cFormat( "Unable to open file : ", FileName ) );
	}
}

void FASTCALL CFileStream::Close()
{
	if ( IsOpen() )
	{
		CloseHandle( FHandle );
		FHandle = INVALID_HANDLE_VALUE;
	}
}

void FASTCALL CFileStream::SetSize( long new_size )
{
	Seek( new_size, po_begin );
	Win32Check( SetEndOfFile( FHandle ) );
}

long FASTCALL CFileStream::Read( void *buffer, long count )
{
	long	result;

	if ( (result = FileRead( (int)FHandle, buffer, count )) == -1 )
		result = 0;
	return ( result );
}

long FASTCALL CFileStream::Write( const void *buffer, long count )
{
	long	result;

	if ( (result = FileWrite( (int)FHandle, buffer, count )) == -1 )
		result = 0;
	return ( result );
}

long FASTCALL CFileStream::Seek( long offset, smode origin )
{
	return( FileSeek( (int)FHandle, (int)offset, origin ) );
}
//---------------------------------------------------------------------------
//--	CCustomMemoryStream
//---------------------------------------------------------------------------
FASTCALL CCustomMemoryStream::CCustomMemoryStream( void *ptr, int size )
	: CStream(), FMem(reinterpret_cast<char *>(ptr)),
	  FEnd(reinterpret_cast<char *>(ptr)+size), FPtr(reinterpret_cast<char *>(ptr))
{
}

long FASTCALL CCustomMemoryStream::GetPosition()
{
	return ( InternalGetPosition() );
}

long FASTCALL CCustomMemoryStream::GetSize()
{
	return ( InternalGetSize() );
}

long FASTCALL CCustomMemoryStream::Read( void *buffer, long count )
{
	long	result;

	if ( (result = FEnd - FPtr) > 0 && count > 0 )
	{
		if ( count < result )
			result = count;
		memcpy( buffer, FPtr, result );
		FPtr += result;
		return ( result );
	}
	return ( 0 );
}

long FASTCALL CCustomMemoryStream::Seek( long offset, smode origin )
{
	switch ( origin )
	{
		case po_begin	: CheckedAdvance( FMem + offset );			break;
		case po_current	: CheckedAdvance( FPtr + offset );			break;
		case po_end		: CheckedAdvance( FEnd - offset - 1 );		break;
	}
	return ( InternalGetPosition() );
}

long FASTCALL CCustomMemoryStream::Write( const void *buffer, long count )
{
	if ( count > 0 )
	{
		if ( FPtr + count > FEnd )
			count = FEnd - FPtr;
		memcpy( FPtr, buffer, count );
		FPtr += count;
	}
	return ( count );
}

void FASTCALL CCustomMemoryStream::LoadFromStream( CStream& stream )
{
	long	count;

	stream.Position = 0;
	count = stream.Size;
	if ( count != 0  )
		stream.ReadBuffer( FMem, count );
}

void FASTCALL CCustomMemoryStream::LoadFromFile( const char *& file_name )
{
	CFileStream		stream( file_name, fm_read | fm_s_deny_write );
	LoadFromStream( stream );
}

void FASTCALL CCustomMemoryStream::SaveToStream( CStream& stream )
{
	if ( InternalGetSize() != 0 )
		stream.WriteBuffer( FMem, InternalGetSize() );
}

void FASTCALL CCustomMemoryStream::SaveToFile( const char *& file_name )
{
	CFileStream		stream( file_name, fm_create );
	SaveToStream( stream );
}
//---------------------------------------------------------------------------
//--	CMemoryStream
//---------------------------------------------------------------------------
FASTCALL CMemoryStream::CMemoryStream()
	: CStream(), FMemory(NULL), FSize(0), FPosition(0), FCapacity(0)
{
}

void FASTCALL CMemoryStream::SetSize( long new_size )
{
	long	old_position;

	old_position = FPosition;
	SetCapacity( new_size );
	FSize = new_size;
	if ( old_position > new_size )
		Seek( 0, po_end );
}

void * FASTCALL CMemoryStream::Realloc( int new_capacity )
{
	void	*result;

	if ( new_capacity > 0 )
		new_capacity = ( new_capacity + (MemoryDelta-1) ) & -MemoryDelta;
	result = Memory;
	if ( new_capacity != FCapacity )
		if ( (result = realloc( FMemory, new_capacity )) == NULL && new_capacity != 0 )
			throw ccEStreamError( "Out of memory" );
	return ( result );
}

void FASTCALL CMemoryStream::SetPointer( void *ptr, int size )
{
	FMemory = ptr;
	FSize = size;
}

FASTCALL CMemoryStream::~CMemoryStream()
{
	Clear();
}

long FASTCALL CMemoryStream::Read( void *buffer, long count )
{
	long	result;

	if ( FPosition >= 0 && count >= 0 )
	{
		if ( (result = FSize - FPosition) > 0 )
		{
			if ( result > count )
				result = count;
			memcpy( buffer, (char *)(FMemory) + FPosition, result );
			FPosition += result;
			return ( result );
		}
	}
	return ( 0 );
}

long FASTCALL CMemoryStream::Seek( long offset, smode origin )
{
	switch ( origin )
	{
		case po_begin	: FPosition = offset;						break;
		case po_current	: FPosition += offset;						break;
		case po_end		: FPosition = FSize + offset;				break;
	}
	return ( FPosition );
}

long FASTCALL CMemoryStream::Write( const void *buffer, long count )
{
	long	pos;

	if ( FPosition >= 0 && count >= 0 )
	{
		if ( (pos = FPosition + count) > 0 )
		{
			if ( pos > FSize )
			{
				if ( pos > FCapacity )
					SetCapacity( pos );
				FSize = pos;
			}
			memcpy( (char *)(FMemory) + FPosition, buffer, count );
			FPosition = pos;
			return ( count );
		}
	}
	return ( 0 );
}

void FASTCALL CMemoryStream::Clear()
{
	SetCapacity( 0 );
	FSize = 0;
	FPosition = 0;
}

void FASTCALL CMemoryStream::SetCapacity( int new_capacity )
{
	SetPointer( Realloc( new_capacity ), FSize );
	FCapacity = new_capacity;
}

void FASTCALL CMemoryStream::LoadFromStream( CStream& stream )
{
	long	count;

	stream.Position = 0;
	count = stream.Size;
	SetSize( count );
	if ( count != 0  )
		stream.ReadBuffer( FMemory, count );
}

void FASTCALL CMemoryStream::LoadFromFile( const char *& file_name )
{
	CFileStream		stream( file_name, fm_read | fm_s_deny_write );
	LoadFromStream( stream );
}

void FASTCALL CMemoryStream::SaveToStream( CStream& stream )
{
	if ( FSize != 0 )
		stream.WriteBuffer( FMemory, FSize );
}

void FASTCALL CMemoryStream::SaveToFile( const char *& file_name )
{
	CFileStream		stream( file_name, fm_create );
	SaveToStream( stream );
}
//---------------------------------------------------------------------------

}; // end namespace cc

