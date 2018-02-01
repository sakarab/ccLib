//---------------------------------------------------------------------------
#ifndef ScanDirH
#define ScanDirH
//---------------------------------------------------------------------------

#include <System.hpp>
#include <vector>
#include "ccStdString.h"

namespace vcl
{

class TScanDir
{
private:
	class FindData
	{
	public:
		HANDLE				fh;
		WIN32_FIND_DATA		data;
		FindData() : fh(INVALID_HANDLE_VALUE)			{ /* empty */ }
		~FindData()
		{
			if ( fh != INVALID_HANDLE_VALUE )
				FindClose( fh );
		}
		void Close()
		{
			if ( fh != INVALID_HANDLE_VALUE )
				FindClose( fh );
			fh = INVALID_HANDLE_VALUE;
		}
	};
	std::vector<FindData *>		FDirList;
	String                      FStartDir;
	String                      FCurrentDir;
	bool						FRunning;
	bool						FRecurcive;
    DWORD GetAttributes();
	DWORD GetFileSize();
	TDateTime GetCreationTime();
	TDateTime GetLastAccessTime();
	TDateTime GetLastWriteTime();
	String GetLongFileName();
	String GetShortFileName();
	String GetBaseDirectory();
	TDateTime FileTimeToDateTime( FILETIME *ttime );
	void Execute();
	void EnterDirectory();
	FindData& GetCurrentData()		                { return ( *FDirList.back() ); }
protected:
	TScanDir( const TScanDir& rhs );				//				{ /* empty */ }
	TScanDir& operator=( const TScanDir& rhs );	//				{ return ( *this ); }
public:
	typedef enum				{ sdfrCancel, sdfrSkip, sdfrContinue } TFindDirResult;
	typedef	TFindDirResult (__closure * TsdfFindDirFunc)( TScanDir *Sender );
	typedef	bool (__closure * TScanDirFunc)( TScanDir *Sender );

	TScanDirFunc				OnFindFile;
	TsdfFindDirFunc				OnFindDir;
	TScanDirFunc				OnDirEnter;
	TScanDirFunc				OnDirExit;
	TScanDirFunc				OnDirExited;
	std::vector<String>         FileSpecList;
	void						*Data;
	TScanDir();
	void Run( const String& start_dir, bool recurcive );
	void Run( stru::char_type *start_dir, bool recurcive );

    static bool IsDirLink( stru::char_type *fname );

	__property DWORD FileSize = { read=GetFileSize };
    __property DWORD Attributes = { read=GetAttributes };
	__property TDateTime CreationTime = { read=GetCreationTime };
	__property TDateTime LastAccessTime = { read=GetLastAccessTime };
	__property TDateTime LastWriteTime = { read=GetLastWriteTime };
	__property String LongFileName = { read=GetLongFileName };
	__property String ShortFileName = { read=GetShortFileName };
	__property String BaseDirectory = { read=GetBaseDirectory };
	__property String CurrentDir = { read=FCurrentDir };
};

}; // end namespace vcl


//---------------------------------------------------------------------------
#endif
