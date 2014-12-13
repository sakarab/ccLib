//---------------------------------------------------------------------------
#pragma hdrstop

#include <SysUtils.hpp>
#include <dir.h>
#include "ScanDir.h"
#include "ccUtils.h"

namespace vcl
{

//---------------------------------------------------------------------------
// static
bool FASTCALL TScanDir::IsDirLink( stru::char_type *fname )
{
	if ( *fname++ == '.' && *fname == 0 )
		return ( true );
	else if ( *fname++ == '.' && *fname == 0 )
		return ( true );
	return ( false );
}
//---------------------------------------------------------------------------
FASTCALL TScanDir::TScanDir()
{
	FRunning = false;
	OnFindFile = NULL;
	OnFindDir = NULL;
	OnDirEnter = NULL;
	OnDirExit = NULL;
	OnDirExited = NULL;
}
//---------------------------------------------------------------------------
DWORD FASTCALL TScanDir::GetFileSize( void )
{
	return GetCurrentData().data.nFileSizeLow;
}
//---------------------------------------------------------------------------
DWORD FASTCALL TScanDir::GetAttributes()
{
	return GetCurrentData().data.dwFileAttributes;
}
//---------------------------------------------------------------------------
TDateTime FASTCALL TScanDir::FileTimeToDateTime( FILETIME *ttime )
{
	FILETIME	file_time;
	SYSTEMTIME	system_time;

	FileTimeToLocalFileTime( ttime, &file_time );
	FileTimeToSystemTime( &file_time, &system_time );
	return ( EncodeDate( system_time.wYear, system_time.wMonth, system_time.wDay ) +
			 EncodeTime( system_time.wHour, system_time.wMinute, system_time.wSecond, system_time.wMilliseconds ) );
}
//---------------------------------------------------------------------------
TDateTime FASTCALL TScanDir::GetCreationTime()
{
	return FileTimeToDateTime( &(GetCurrentData().data.ftCreationTime) );
}
//---------------------------------------------------------------------------
TDateTime FASTCALL TScanDir::GetLastAccessTime()
{
	return FileTimeToDateTime( &(GetCurrentData().data.ftLastAccessTime) );
}
//---------------------------------------------------------------------------
TDateTime FASTCALL TScanDir::GetLastWriteTime()
{
	return FileTimeToDateTime( &(GetCurrentData().data.ftLastWriteTime) );
}
//---------------------------------------------------------------------------
String FASTCALL TScanDir::GetLongFileName()
{
	return GetCurrentData().data.cFileName;
}
//---------------------------------------------------------------------------
String FASTCALL TScanDir::GetShortFileName()
{
	return GetCurrentData().data.cAlternateFileName;
}
//---------------------------------------------------------------------------
String FASTCALL TScanDir::GetBaseDirectory()
{
	return ( FCurrentDir.c_str() + FStartDir.Length() );
}
//---------------------------------------------------------------------------
void FASTCALL TScanDir::Run( const String& start_dir, bool recurcive )
{
	if ( FRunning == false )
	{
		FindData	dta;

		FRunning = true;
		FRecurcive = recurcive;
		FCurrentDir = start_dir;
		if ( FCurrentDir[FCurrentDir.Length()] != '\\' )
			FCurrentDir += "\\";
		FStartDir = FCurrentDir;
		FDirList.push_back( &dta );
		try
		{
			Execute();
		}
		__finally
		{
			FDirList.clear();
			FRunning = false;
		}
	}
}
void FASTCALL TScanDir::Run( stru::char_type *start_dir, bool recurcive )
{
	Run( String( start_dir ), recurcive );
}
//---------------------------------------------------------------------------
static int split_fname( stru::char_type *name, stru::char_type *store_base, stru::char_type *store_ext )
{
	stru::char_type     drive[MAXDRIVE], dir[MAXDIR];

	return stru::in_fnsplit( name, drive, dir, store_base, store_ext );
}

static bool fname_cmp( stru::char_type *name, stru::char_type *spec )
{
	do
	{
		if ( *spec == '*' )
			break;
		if ( *spec == '?' )
			name++;
		else if ( *spec != *name++ )
			return( true );
	}
	while( *spec++ );
	return( false );
}

static bool fname_match( stru::char_type *name, stru::char_type *spec )
{
	stru::char_type     cname[MAXFILE], cext[MAXEXT];
	stru::char_type     st_name[MAXFILE], st_ext[MAXEXT];

	split_fname( name, cname, cext );
	split_fname( spec, st_name, st_ext );
	CharUpper( cname );
	CharUpper( cext );
	CharUpper( st_name );
	CharUpper( st_ext );
	if ( !fname_cmp( cname, st_name ) )
		return( !fname_cmp( cext, st_ext ) );
	return( false );
}

void FASTCALL TScanDir::Execute()
{
	bool				stop;
	TFindDirResult	    fdr;
	FindData&			cur_data( GetCurrentData() );

	if ( OnDirEnter != NULL && OnDirEnter( this ) == false )
		Abort();

	if ( (cur_data.fh = FindFirstFile( (FCurrentDir + "*.*").c_str(), &cur_data.data )) == INVALID_HANDLE_VALUE )
	{
		if ( reinterpret_cast<LONG>(cur_data.fh) != ERROR_FILE_NOT_FOUND )
			ThrowException( TEXT("Error reading directory") );
		stop = true;
	}
	else
		stop = false;

	while ( stop == false )
	{
		if ( (cur_data.data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			if ( FRecurcive )
			{
				fdr = sdfrContinue;
				if ( OnFindDir != NULL )
					fdr = OnFindDir( this );
				switch ( fdr )
				{
					case sdfrCancel :
						Abort();
						break;
					case sdfrContinue :
						if ( ! IsDirLink( cur_data.data.cFileName ) )
							EnterDirectory();
						break;
				}
			}
		}
		else if ( OnFindFile != NULL )
		{
			for ( std::vector<String>::iterator n = FileSpecList.begin() ; n != FileSpecList.end() ; ++n )
				if ( fname_match( cur_data.data.cFileName, n->c_str() ) )
				{
					if ( OnFindFile( this ) == false )
						Abort();
					break;
				}
		}
		if ( FindNextFile( cur_data.fh, &cur_data.data ) == 0 )
			if ( GetLastError() == ERROR_NO_MORE_FILES )
				stop = true;
			else
				ThrowException( TEXT("Error reading directory") );
	}
	if ( OnDirExit != NULL && OnDirExit( this ) == false )
		Abort();
}
//---------------------------------------------------------------------------
void FASTCALL TScanDir::EnterDirectory()
{
	stru::char_type     *tmpstr;
	FindData            dta;

	FCurrentDir = FCurrentDir + GetCurrentData().data.cFileName + '\\';

	FDirList.push_back( &dta );
	Execute();
	FDirList.pop_back();
	dta.Close();

	FCurrentDir.Delete( FCurrentDir.Length(), 1 );
	if ( (tmpstr = stru::in_strrchr( FCurrentDir.c_str(), TEXT('\\') ) ) != NULL )
		FCurrentDir.SetLength( tmpstr - FCurrentDir.c_str() + 1 );
	if ( OnDirExited != NULL && OnDirExited( this ) == false )
		Abort();
}
//---------------------------------------------------------------------------

}; // end namespace vcl

