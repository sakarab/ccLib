//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include <stdio>
#include <UtilCls.h>
#include "ccUtils.h"
#include <string>
//---------------------------------------------------------------------------

String AddSlash( const String& fname )
{
	String	    result;

	if ( ! fname.IsEmpty() )
	{
		result = fname;
		if ( result[result.Length() - 1] != '\\' )
			result += String( TEXT("\\") );
	}
	else
		result = TEXT("\\");
	return result;
}

void AddSlash( stru::char_type *fname )
{
	if ( fname != NULL )
	{
		if ( *fname != TEXT('\0') )
		{
			fname += ( stru::in_strlen( fname ) - 1 );
			if ( *fname != TEXT('\\') )
				stru::in_strcpy( fname + 1, TEXT("\\") );
		}
		else
			stru::in_strcpy( fname, TEXT("\\") );
	}
}

String BaseName( const String& sstr )
{
	stru::char_type     *c_str = sstr.c_str(), *start_str, *end_str;

	if ( ( start_str = stru::in_strrchr( c_str, TEXT('\\') ) ) == NULL )
		start_str = c_str;
	else
		start_str++;
	if ( ( end_str = stru::in_strrchr( start_str, TEXT('.') ) ) == NULL )
		end_str = c_str + sstr.Length();
	return String( start_str, end_str - start_str );
}

String GetWinVersionInfo()
{
	OSVERSIONINFO	osver;

	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &osver );

    stru::char_type     *win_descr = TEXT("");

	switch ( osver.dwPlatformId )
	{
		case VER_PLATFORM_WIN32s :
			win_descr = TEXT("Windows 32s");
			break;
		case VER_PLATFORM_WIN32_WINDOWS :
			win_descr = TEXT("Windows 95");
			break;
		case VER_PLATFORM_WIN32_NT :
			win_descr = TEXT("Windows NT");
			break;
	}

	String          result;

    result.sprintf( TEXT("%s, %d.%d %s"), win_descr, (int)osver.dwMajorVersion,
                                          (int)osver.dwMinorVersion, osver.szCSDVersion );
	return result;
}

String GetDllVersion( const String& fname )
{
	int					len;
	DWORD				dummy;
	UINT				query_len;
	VS_FIXEDFILEINFO	*version;
	String              result;

	if ( (len = GetFileVersionInfoSize( fname.c_str(), &dummy )) == 0 )
		ThrowException( TEXT("Unable to read version info.") );

	TAPtr<char>		tmp_buff( new char[len + 30] );

	if ( GetFileVersionInfo( fname.c_str(), 0, len, tmp_buff ) == FALSE )
		ThrowException( TEXT("Unable to read version info.") );
	VerQueryValue( tmp_buff, TEXT("\\"), reinterpret_cast<void **>(&version), &query_len );
	result.sprintf( TEXT("%d.%d.%d.%d"),
					version->dwFileVersionMS >> 16,
					version->dwFileVersionMS & 0x0000FFFF,
					version->dwFileVersionLS >> 16,
					version->dwFileVersionLS & 0x0000FFFF );
	return result;
}

void ThrowException( stru::char_type *mess )
{
	throw Exception( mess );
}

void ThrowExceptionFmt( stru::char_type *fmt, ... )
{
    String      str;
	va_list		list;

	va_start(list,fmt);
    str.vprintf( fmt, list );
	va_end(list);
	throw Exception( str );
}

String MakeTempFile()
{
	stru::char_type     fname[MAX_PATH], fpath[MAX_PATH];

	GetTempPath( MAX_PATH, fpath );
	GetTempFileName( fpath, TEXT("mtf"), 0, fname );
	return String( fname );
}

