//---------------------------------------------------------------------------

#ifndef FCTreeH
#define FCTreeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CTree.h"
#include "ScanDir.h"
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
//--	CTree
//---------------------------------------------------------------------------
class Data
{
public:
	enum FileType{ unknown, dir, file };
private:
	FileType	type;
	AnsiString	FName;
public:
	Data();
	Data( const Data& src );
	Data( FileType a, const AnsiString& b );
	~Data();
	Data& operator=( const Data& src );
    AnsiString& GetName()                           { return ( FName ); }
};

typedef	cc::CTree<Data>		FileTree;
//typedef	cc::tree<Data>		FileTree;
//---------------------------------------------------------------------------
class TfrmCTree : public TForm
{
__published:	// IDE-managed Components
	TButton *btnCTree;
	TOpenDialog *OpenDlg;
    TTreeView *tvTree;
	void __fastcall btnCTreeClick(TObject *Sender);
private:	// User declarations
	AnsiString __fastcall SelectDir();
	bool OnFindFile( vcl::TScanDir *scan_dir );
	vcl::TScanDir::TFindDirResult OnFindDir( vcl::TScanDir *scan_dir );
	bool OnDirExited( vcl::TScanDir *scan_dir );
    void __fastcall FillTreeView( FileTree& file_tree );
public:		// User declarations
	__fastcall TfrmCTree(TComponent* Owner);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
