//---------------------------------------------------------------------------
#ifndef StlStrH
#define StlStrH
//---------------------------------------------------------------------------
#include "ssport.h"
#include <SysUtils.hpp>
#include <string>

namespace cc
{

template <class charT, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT> >
class Str : public std::basic_string<charT, traits, Allocator>
{
private:
protected:
public:
	FASTCALL Str()
		: std::basic_string<charT, traits, Allocator>()							{ /* empty */ }
	FASTCALL Str( const Str<charT, traits, Allocator>& str )
		: std::basic_string<charT, traits, Allocator>( str )					{ /* empty */ }
	FASTCALL Str( const Str<charT, traits, Allocator>& str,
				  std::basic_string<charT, traits, Allocator>::size_type pos,
				  std::basic_string<charT, traits, Allocator>::size_type n=npos )
		: std::basic_string<charT, traits, Allocator>( str, pos, n )			{ /* empty */ }
	FASTCALL Str( const char *str )
		: std::basic_string<charT, traits, Allocator>( str )					{ /* empty */ }
	FASTCALL Str( const char *str,
				  std::basic_string<charT, traits, Allocator>::size_type n )
		: std::basic_string<charT, traits, Allocator>( str, n )					{ /* empty */ }
	FASTCALL Str( AnsiString str )
		: std::basic_string<charT, traits, Allocator>( str.c_str() )			{ /* empty */ }

	FASTCALL Str( const char src )
		: std::basic_string<charT, traits, Allocator>( 1, src )					{ /* empty */ }
	FASTCALL Str( const short int src )
	{
		sprintf( "%hd", src );
	}
	FASTCALL Str( const unsigned short src )
	{
		sprintf( "%hu", src );
	}
	FASTCALL Str( const int src )
	{
		sprintf( "%i", src );
	}
	FASTCALL Str( const unsigned int src )
	{
		sprintf( "%u", src );
	}
	FASTCALL Str( const long src )
	{
		sprintf( "%ld", src );
	}
	FASTCALL Str( const unsigned long src )
	{
		sprintf( "%lu", src );
	}
	FASTCALL Str( const __int64 src )
	{
		sprintf( "%Li", src );
	}
	FASTCALL Str( const unsigned __int64 src )
	{
		sprintf( "%Lu", src );
	}
	FASTCALL Str( const double src )
	{
		char	buffer[64];

		FloatToText( buffer, &src, fvExtended, ffGeneral, 15, 0 );
		*this = buffer;
	}

	Str<charT, traits, Allocator>& FASTCALL operator = ( const Str<charT, traits, Allocator>& str )
	{
		std::basic_string<charT, traits, Allocator>::operator=( str );
		return ( *this );
	}
	Str<charT, traits, Allocator>& FASTCALL operator = ( const AnsiString& str )
	{
		std::basic_string<charT, traits, Allocator>::operator=( str.c_str() );
		return ( *this );
	}
	Str<charT, traits, Allocator>& FASTCALL operator = ( const char *str )
	{
		std::basic_string<charT, traits, Allocator>::operator=( str );
		return ( *this );
	}
	Str<charT, traits, Allocator>& FASTCALL operator = ( const char str )
	{
		std::basic_string<charT, traits, Allocator>::operator=( str );
		return ( *this );
	}
};

#pragma warn -8026
#pragma warn -8027
class cStr : public std::string
{
private:
protected:
public:
	FASTCALL cStr() : std::string()												{ /* empty */ }
	FASTCALL cStr( const cStr& str ) : std::string( str )						{ /* empty */ }
	FASTCALL cStr( const cStr& str, size_type pos, size_type n=npos )
		: std::string( str, pos, n )											{ /* empty */ }
	FASTCALL cStr( const char *str ) : std::string( str )						{ /* empty */ }
	FASTCALL cStr( const char *str, size_type n ) : std::string( str, n )		{ /* empty */ }
	FASTCALL cStr( AnsiString str ) : std::string( str.c_str() )				{ /* empty */ }

	FASTCALL cStr( char src );
	FASTCALL cStr( short src );
	FASTCALL cStr( unsigned short src );
	FASTCALL cStr( int src );
	FASTCALL cStr( unsigned int src );
	FASTCALL cStr( long src );
	FASTCALL cStr( unsigned long src );
	FASTCALL cStr( __int64 src );
	FASTCALL cStr( unsigned __int64 src );
	FASTCALL cStr( double src );

	cStr& FASTCALL operator = ( const cStr& str );
	cStr& FASTCALL operator = ( const AnsiString& str );
	cStr& FASTCALL operator = ( const char *str );
	cStr& FASTCALL operator = ( const char str );

	FASTCALL operator AnsiString()
	{
		return ( AnsiString( data() ) );
	}

	// C style 'sprintf' (NOTE: Target buffer is the string)
	cStr& __cdecl  sprintf( const char* format, ... );			// Returns *this
	int  __cdecl   printf( const char* format, ... );			// Returns formatted length
	int  __cdecl   vprintf( const char* format, va_list );		// Returns formatted length
	// Like above, but appends to the string rather than overwrite
	cStr& __cdecl  cat_sprintf( const char* format, ... );		// Returns *this
	int   __cdecl  cat_printf( const char* format, ... );		// Returns formatted length
	int   __cdecl  cat_vprintf( const char* format, va_list );	// Returns formatted length
};
#pragma warn +8026
#pragma warn +8027

}; // end namespace cc

//---------------------------------------------------------------------------
#endif
