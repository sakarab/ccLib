//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ccLibTest.res");
USEFORM("Test\FMain.cpp", frmMain);
USEFORM("Test\FCTree.cpp", frmCTree);
USEFORM("Test\FCList.cpp", frmCList);
USELIB("G:\Lib\ccLib50s.lib");
USEFORM("Test\FRefCnt.cpp", frmRefCnt);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
