//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Forms.hpp>
#include <Consts.hpp>
// BCB_XE5 #include <WideStrUtils.hpp>
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

bool FASTCALL CCustomIniFile::ReadBool( const stru::char_type *Section, const stru::char_type *Ident, bool Default )
{
	return ( ReadInteger( Section, Ident, Default ) != 0 );
}

long FASTCALL CCustomIniFile::ReadInteger( const stru::char_type *Section, const stru::char_type *Ident, long Default )
{
	String      IntStr( ReadString( Section, Ident, TEXT("") ) );

	if ( IntStr.Length() > 2 && IntStr[1] == '0' && (IntStr[2] == 'X' || IntStr[2] == 'x') )
		IntStr = String( "$" ) + IntStr.SubString( 3, IntStr.Length() );
	return StrToIntDef( IntStr, Default );
}

TDateTime FASTCALL CCustomIniFile::ReadDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Default )
{
	String      str( ReadString( Section, Ident, TEXT("") ) );
	TDateTime	result( Default );

	if ( ! str.IsEmpty() )
	{
		try							{ result = StrToDateTime( str ); }
		catch ( EConvertError& )	{ }
	}
	return ( result );
}

double FASTCALL CCustomIniFile::ReadFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Default )
{
	String      str( ReadString( Section, Ident, TEXT("") ) );
	double		result = Default;

	if ( ! str.IsEmpty() )
	{
		try							{ result = StrToFloat( str ); }
		catch ( EConvertError& )	{ }
	}
	return ( result );
}


void FASTCALL CCustomIniFile::ReadSection( const stru::char_type *Section, std::vector<String>& strings )
{
    std::vector<stru::char_type>    buffer( 16384 );

    strings.clear();
    if ( GetPrivateProfileString( Section, NULL, NULL, &buffer.front(),
                                  buffer.size(), GetFileName().c_str() ) != 0 )
    {
        stru::char_type     *p = &buffer.front();

        while ( *p != 0 )
        {
            strings.push_back( String( p ) );
            p += stru::in_strlen( p ) + 1;
        }
    }
}

void FASTCALL CCustomIniFile::WriteBool( const stru::char_type *Section, const stru::char_type *Ident, bool Value )
{
	WriteString( Section, Ident, Value ? TEXT("1") : TEXT("0") );
}

void FASTCALL CCustomIniFile::WriteInteger( const stru::char_type *Section, const stru::char_type *Ident, long Value )
{
	WriteString( Section, Ident, IntToStr( static_cast<__int64>(Value) ).c_str() );
}

void FASTCALL CCustomIniFile::WriteDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Value )
{
    WriteString( Section, Ident, DateTimeToStr( Value ).c_str() );
}

void FASTCALL CCustomIniFile::WriteFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Value )
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

String FASTCALL CIniFile::ReadString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Default )
{
	stru::char_type     buff[2048];

	return String( buff, GetPrivateProfileString( Section, Ident, Default,
                                                  buff, sizeof(buff), GetFileName().c_str() ) );
}

void FASTCALL CIniFile::WriteString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Value )
{
	if ( ! WritePrivateProfileString( Section, Ident, Value, GetFileName().c_str() ) )
		throw Exception( TEXT("Ini file write error.") );
}
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------

