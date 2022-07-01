//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FMain.h"
#include "FCTree.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Data::Data()
	: type(unknown), FName(EmptyStr)
{
}

Data::Data( const Data& src )
	: type(src.type), FName(src.FName)
{
}

Data::Data( FileType a, const AnsiString& b )
	: type(a), FName(b)
{
}

Data::~Data()
{
}

Data& Data::operator=( const Data& src )
{
	if ( this != &src )
	{
		type = src.type;
		FName = src.FName;
	}
	return ( *this );
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfrmMain::SelectDir()
{
	AnsiString	result( OpenDlg->FileName );

	if ( result.IsEmpty() )
		OpenDlg->FileName = "Filename will not be used";
	if ( !OpenDlg->Execute() )
		result = EmptyStr;
	else
		result = ExtractFilePath( OpenDlg->FileName );
	return ( result );
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::OnFindFile( TScanDir *scan_dir )
{
	FileTree	**tree = reinterpret_cast<FileTree **>(&scan_dir->Data);

	(*tree)->Add( Data( Data::file, scan_dir->LongFileName ) );
	return ( true );
}
//---------------------------------------------------------------------------
TScanDir::TsdfFindDirResult __fastcall TfrmMain::OnFindDir( TScanDir *scan_dir )
{
	FileTree			**tree = reinterpret_cast<FileTree **>(&scan_dir->Data);

	if ( scan_dir->LongFileName[1] != '.' )
		scan_dir->Data = &(*(*tree)->Add( Data( Data::dir, scan_dir->LongFileName ) ));
	return ( TScanDir::sdfrContinue );
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::OnDirExited( TScanDir *scan_dir )
{
	FileTree	**tree = reinterpret_cast<FileTree **>(&scan_dir->Data);

	scan_dir->Data = (*tree)->parent();
	return ( true );
}
//---------------------------------------------------------------------------

