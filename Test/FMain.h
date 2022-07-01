//---------------------------------------------------------------------------
#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
//--	TfrmMain
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TButton *btnCTree;
	TButton *btnCList;
    TButton *btnRefCnt;
	void __fastcall btnCTreeClick(TObject *Sender);
	void __fastcall btnCListClick(TObject *Sender);
    void __fastcall btnRefCntClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
