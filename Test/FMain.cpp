//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FMain.h"
#include "FCTree.h"
#include "FCList.h"
#include "FRefCnt.h"
#include <memory>
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCTreeClick(TObject *Sender)
{
	std::auto_ptr<TfrmCTree>	frm( new TfrmCTree( NULL ) );
	frm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCListClick(TObject *Sender)
{
	std::auto_ptr<TfrmCList>	frm( new TfrmCList( NULL ) );
	frm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRefCntClick(TObject *Sender)
{
	std::auto_ptr<TfrmRefCnt>	frm( new TfrmRefCnt( NULL ) );
	frm->ShowModal();
}
//---------------------------------------------------------------------------

