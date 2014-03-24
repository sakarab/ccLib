//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "StlStr.h"
#include <stdio>
//---------------------------------------------------------------------------

namespace cc
{

FASTCALL cStr::cStr( char src ) : std::string( 1, src )
{
}

FASTCALL cStr::cStr( short src ) : std::string()
{
	sprintf( "%hd", src );
}

FASTCALL cStr::cStr( unsigned short src ) : std::string()
{
	sprintf( "%hu", src );
}

FASTCALL cStr::cStr( int src ) : std::string()
{
	sprintf( "%i", src );
}

FASTCALL cStr::cStr( unsigned int src ) : std::string()
{
	sprintf( "%u", src );
}

FASTCALL cStr::cStr( long src ) : std::string()
{
	sprintf( "%ld", src );
}

FASTCALL cStr::cStr( unsigned long src ) : std::string()
{
	sprintf( "%lu", src );
}

FASTCALL cStr::cStr( __int64 src ) : std::string()
{
	sprintf( "%Li", src );
}

FASTCALL cStr::cStr( unsigned __int64 src ) : std::string()
{
	sprintf( "%Lu", src );
}

FASTCALL cStr::cStr( double src ) : std::string()
{
	char	buffer[64];

	FloatToText( buffer, &src, fvExtended, ffGeneral, 15, 0 );
	*this = buffer;
}

cStr& FASTCALL cStr::operator = ( const cStr& str )
{
	std::string::operator=( str );
	return ( *this );
}

cStr& FASTCALL cStr::operator = ( const AnsiString& str )
{
	std::string::operator=( str.c_str() );
	return ( *this );
}

cStr& FASTCALL cStr::operator = ( const char *str )
{
	std::string::operator=( str );
	return ( *this );
}

cStr& FASTCALL cStr::operator = ( const char str )
{
	std::string::operator=( str );
	return ( *this );
}

int __cdecl cStr::vprintf( const char* format, va_list paramList )
{
	int		size = vsnprintf( NULL, 0, format, paramList );

	resize( size );
	return vsnprintf( const_cast<char *>(data()), size, format, paramList );
}

int __cdecl cStr::cat_vprintf( const char* format, va_list paramList )
{
	int		size = vsnprintf( NULL, 0, format, paramList );
	int		len = length();

	resize( len + size );
	return vsnprintf( const_cast<char *>(data()) + len, len + size, format, paramList );
}

int __cdecl cStr::printf( const char* format, ... )
{
	int			rc;
	va_list		paramList;

	va_start(paramList, format);
	rc = vprintf( format, paramList );
	va_end( paramList );
	return rc;
}

cStr& __cdecl cStr::sprintf( const char* format, ... )
{
	va_list		paramList;

	va_start(paramList, format);
	vprintf( format, paramList );
	va_end(paramList);
	return *this;
}

int __cdecl cStr::cat_printf( const char* format, ... )
{
	int			rc;
	va_list		paramList;

	va_start(paramList, format);
	rc = cat_vprintf( format, paramList );
	va_end(paramList);
	return rc;
}

cStr& __cdecl cStr::cat_sprintf( const char* format, ... )
{
	va_list		paramList;

	va_start(paramList, format);
	cat_vprintf( format, paramList );
	va_end(paramList);
	return *this;
}

}; // end namespace cc

