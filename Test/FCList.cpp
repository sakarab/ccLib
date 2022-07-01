//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FCList.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmCList *frmCList;
//---------------------------------------------------------------------------
__fastcall TfrmCList::TfrmCList(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TfrmCList::~TfrmCList()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCList::btnFillClick(TObject *Sender)
{
    cc::CList<int>   a, b;
    cc::CList<int>&  aa = a;
    cc::CList<int>&  bb = b;

	for ( int n = 10 ; n >= 0 ; --n )
		FData.Add( new int(n) );
}
//---------------------------------------------------------------------------
void __fastcall TfrmCList::btnClearClick(TObject *Sender)
{
	FData.Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCList::Display()
{
	lbList->Items->Clear();
	for ( int n = 0 ; n < FData.GetCount() ; ++n )
		lbList->Items->Add( IntToStr( *FData.GetItem( n ) ) );
}
//---------------------------------------------------------------------------
int __fastcall TfrmCList::TestIterator()
{
	int		sum = 0;

	for ( cc::CList<int>::iterator ci = FData.begin() ; ci != FData.end() ; ++ci )
	{
		int		n = *ci;
		int		*n_ptr = ci.operator->();

		if ( n & 0x1 )
			sum += n;
		else
			sum += *n_ptr;
	}
	return ( sum );
}

void __fastcall TfrmCList::btnTestIterClick(TObject *Sender)
{
	TestIterator();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCList::btnStdSortClick(TObject *Sender)
{
	std::sort( FData.begin(), FData.end() );
}
//---------------------------------------------------------------------------
void __fastcall TfrmCList::btnDisplayClick(TObject *Sender)
{
	Display();
}
//---------------------------------------------------------------------------

