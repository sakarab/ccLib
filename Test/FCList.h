//---------------------------------------------------------------------------
#ifndef FCListH
#define FCListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ccList.h"
//---------------------------------------------------------------------------
//--	CList
//---------------------------------------------------------------------------
class ListData
{
public:
	enum FileType{ unknown, dir, file };
private:
	FileType	type;
	AnsiString	FName;
public:
	ListData();
	ListData( const ListData& src );
	ListData( FileType a, const AnsiString& b );
	~ListData();
	ListData& operator=( const ListData& src );
};

//---------------------------------------------------------------------------
class TfrmCList : public TForm
{
__published:	// IDE-managed Components
	TButton *btnFill;
	TButton *btnClear;
	TButton *btnTestIter;
	TButton *btnStdSort;
	TListBox *lbList;
	TButton *btnDisplay;
	void __fastcall btnFillClick(TObject *Sender);
	void __fastcall btnClearClick(TObject *Sender);
	void __fastcall btnTestIterClick(TObject *Sender);
	void __fastcall btnStdSortClick(TObject *Sender);
	void __fastcall btnDisplayClick(TObject *Sender);
private:	// User declarations
	cc::CList<int>	FData;
	int __fastcall TestIterator();
	void __fastcall Display();
public:		// User declarations
	__fastcall TfrmCList(TComponent* Owner);
	virtual __fastcall ~TfrmCList();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
