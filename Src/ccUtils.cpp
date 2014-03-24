//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include <stdio>
#include "ssport.h"
#include <UtilCls.h>
#include "ccUtils.h"
//---------------------------------------------------------------------------

const char	    EmptyChar[] = "";
const wchar_t	EmptyCharW[] = L"";

String AddSlash( const String& fname )
{
	wchar_t     *str;
	String	    result;

	if ( fname != "" )
	{
		result = fname;
		str = result.c_str() + result.Length() - 1 ;
		if ( *str != '\\' )
			result += String( "\\" );
	}
	else
		result = "\\";
	return result;
}

void AddSlash( wchar_t *fname )
{
	if ( fname != NULL )
	{
		if ( *fname != '\0' )
		{
			fname += ( wcslen( fname ) - 1 );
			if ( *fname != '\\' )
				wcscpy( fname + 1, TEXT("\\") );
		}
		else
			wcscpy( fname, TEXT("\\") );
	}
}

String BaseName( const String& sstr )
{
	wchar_t     *c_str = sstr.c_str(), *start_str, *end_str;

	if ( ( start_str = wcsrchr( c_str, TEXT('\\') ) ) == NULL )
		start_str = c_str;
	else
		start_str++;
	if ( ( end_str = wcsrchr( start_str, TEXT('.') ) ) == NULL )
		end_str = c_str + sstr.Length();
	return String( start_str, end_str - start_str );
}

String GetWinVersionInfo()
{
	OSVERSIONINFO	osver;

	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &osver );

    wchar_t         *win_descr = TEXT("");

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

void ThrowException( wchar_t *mess )
{
	throw Exception( mess );
}

void ThrowExceptionFmt( wchar_t *fmt, ... )
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
	wchar_t     fname[MAX_PATH], fpath[MAX_PATH];

	GetTempPath( MAX_PATH, fpath );
	GetTempFileName( fpath, TEXT("mtf"), 0, fname );
	return String( fname );
}

