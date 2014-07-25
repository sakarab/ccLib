//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Forms.hpp>
#include <Consts.hpp>
#include <WideStrUtils.hpp>
#include "CIniFiles.h"
#include "ccUtils.h"
//---------------------------------------------------------------------------

namespace ww
{
//---------------------------------------------------------------------------
//-- CCustomIniFile
//---------------------------------------------------------------------------
FASTCALL CCustomIniFile::CCustomIniFile()
	: FName( ChangeFileExt( Application->ExeName, ".ini" ) )
{
}

FASTCALL CCustomIniFile::CCustomIniFile( const CCustomIniFile& src )
	: FName(src.FName)
{
}

FASTCALL CCustomIniFile::CCustomIniFile( const String& fname )
	: FName(fname)
{
}

CCustomIniFile& CCustomIniFile::operator=( const CCustomIniFile& src )
{
	if ( this != &src )
		FName = src.FName;
	return ( *this );
}

bool FASTCALL CCustomIniFile::ReadBool( const wchar_t *Section, const wchar_t *Ident, bool Default )
{
	return ( ReadInteger( Section, Ident, Default ) != 0 );
}

long FASTCALL CCustomIniFile::ReadInteger( const wchar_t *Section, const wchar_t *Ident, long Default )
{
	String      IntStr( ReadString( Section, Ident, EmptyCharW ) );

	if ( IntStr.Length() > 2 && IntStr[1] == '0' && (IntStr[2] == 'X' || IntStr[2] == 'x') )
		IntStr = String( "$" ) + IntStr.SubString( 3, IntStr.Length() );
	return StrToIntDef( IntStr, Default );
}

TDateTime FASTCALL CCustomIniFile::ReadDateTime( const wchar_t *Section, const wchar_t *Ident, const TDateTime& Default )
{
	String      str( ReadString( Section, Ident, EmptyCharW ) );
	TDateTime	result( Default );

	if ( ! str.IsEmpty() )
	{
		try							{ result = StrToDateTime( str ); }
		catch ( EConvertError& )	{ }
	}
	return ( result );
}

double FASTCALL CCustomIniFile::ReadFloat( const wchar_t *Section, const wchar_t *Ident, const double& Default )
{
	String      str( ReadString( Section, Ident, EmptyCharW ) );
	double		result = Default;

	if ( ! str.IsEmpty() )
	{
		try							{ result = StrToFloat( str ); }
		catch ( EConvertError& )	{ }
	}
	return ( result );
}


void FASTCALL CCustomIniFile::ReadSection( const wchar_t *Section, std::vector<String>& strings )
{
    std::vector<wchar_t>      buffer( 16384 );

    strings.clear();
    if ( GetPrivateProfileString( Section, NULL, NULL, &buffer.front(),
                                  buffer.size(), GetFileName().c_str() ) != 0 )
    {
        wchar_t    *p = &buffer.front();

        while ( *p != 0 )
        {
            strings.push_back( String( p ) );
            p += WStrLen( p ) + 1;
        }
    }
}

void FASTCALL CCustomIniFile::WriteBool( const wchar_t *Section, const wchar_t *Ident, bool Value )
{
	WriteString( Section, Ident, Value ? TEXT("1") : TEXT("0") );
}

void FASTCALL CCustomIniFile::WriteInteger( const wchar_t *Section, const wchar_t *Ident, long Value )
{
	WriteString( Section, Ident, IntToStr( static_cast<__int64>(Value) ).c_str() );
}

void FASTCALL CCustomIniFile::WriteDateTime( const wchar_t *Section, const wchar_t *Ident, const TDateTime& Value )
{
    WriteString( Section, Ident, DateTimeToStr( Value ).c_str() );
}

void FASTCALL CCustomIniFile::WriteFloat( const wchar_t *Section, const wchar_t *Ident, const double& Value )
{
    WriteString( Section, Ident, FloatToStr( Value ).c_str() );
}
//---------------------------------------------------------------------------
//-- CIniFile
//---------------------------------------------------------------------------
FASTCALL CIniFile::CIniFile()
	: CCustomIniFile()
{
}

FASTCALL CIniFile::CIniFile( String fname )
	: CCustomIniFile(fname)
{
}

String FASTCALL CIniFile::ReadString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Default )
{
	wchar_t     buff[2048];

	return String( buff, GetPrivateProfileString( Section, Ident, Default,
                                                  buff, sizeof(buff), GetFileName().c_str() ) );
}

void FASTCALL CIniFile::WriteString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Value )
{
	if ( ! WritePrivateProfileString( Section, Ident, Value, GetFileName().c_str() ) )
		throw Exception( String().LoadStr( System::Rtlconsts::_SIniFileWriteError.Identifier ) );
}
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------

