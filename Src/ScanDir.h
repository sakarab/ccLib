//---------------------------------------------------------------------------
#ifndef ScanDirH
#define ScanDirH
//---------------------------------------------------------------------------

#include <System.hpp>
#include "ssport.h"
#include <vector>

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
		FASTCALL FindData() : fh(INVALID_HANDLE_VALUE)			{ /* empty */ }
		FASTCALL ~FindData()
		{
			if ( fh != INVALID_HANDLE_VALUE )
				FindClose( fh );
		}
		void FASTCALL Close()
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
    DWORD FASTCALL GetAttributes();
	DWORD FASTCALL GetFileSize();
	TDateTime FASTCALL GetCreationTime();
	TDateTime FASTCALL GetLastAccessTime();
	TDateTime FASTCALL GetLastWriteTime();
	String FASTCALL GetLongFileName();
	String FASTCALL GetShortFileName();
	String FASTCALL GetBaseDirectory();
	TDateTime FASTCALL FileTimeToDateTime( FILETIME *ttime );
	void FASTCALL Execute();
	void FASTCALL EnterDirectory();
	FindData& FASTCALL GetCurrentData()		                { return ( *FDirList.back() ); }
protected:
	FASTCALL TScanDir( const TScanDir& rhs );				//				{ /* empty */ }
	TScanDir& FASTCALL operator=( const TScanDir& rhs );	//				{ return ( *this ); }
public:
	typedef enum				{ sdfrCancel, sdfrSkip, sdfrContinue } TFindDirResult;
	typedef	TFindDirResult (__closure FASTCALL * TsdfFindDirFunc)( TScanDir *Sender );
	typedef	bool (__closure FASTCALL * TScanDirFunc)( TScanDir *Sender );

	TScanDirFunc				OnFindFile;
	TsdfFindDirFunc				OnFindDir;
	TScanDirFunc				OnDirEnter;
	TScanDirFunc				OnDirExit;
	TScanDirFunc				OnDirExited;
	std::vector<String>         FileSpecList;
	void						*Data;
	FASTCALL TScanDir();
	void FASTCALL Run( const String& start_dir, bool recurcive );
	void FASTCALL Run( wchar_t *start_dir, bool recurcive );

    static bool FASTCALL IsDirLink( wchar_t *fname );

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
