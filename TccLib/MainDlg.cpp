// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "cclib_tests.h"
#include <cc_scope_exit.h>
#include <boost/scope_exit.hpp>

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    DlgResize_Init( true, true, WS_THICKFRAME | WS_CLIPCHILDREN );

    // center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

    mLog.Attach( GetDlgItem( IDC_RICHEDIT2_LOG ) );
    mLogger.Attach( mLog );

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD wNotifyCode, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg   dlg;

    dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnClearLog( WORD, WORD wID, HWND, BOOL & )
{
#if defined(CC_HAVE_INLINE_FUNCTORS)
	int		a = 5;
	int		b = 5;

	CC_SCOPE_EXIT( ([&a, &b]{ a = 0; }) );
	CC_SCOPE_EXIT( ([&a, &b]{ a = 0; }) );
#endif
    mLog.SetWindowText( _TEXT("") );
    return LRESULT();
}

LRESULT CMainDlg::OnTestStringList( WORD, WORD wID, HWND, BOOL & )
{
#if defined(CC_HAVE_INLINE_FUNCTORS)
    testStringList( mLog );
#endif
    return LRESULT();
}

LRESULT CMainDlg::OnTestBase64( WORD, WORD wID, HWND, BOOL & )
{
#if defined(CC_HAVE_INLINE_FUNCTORS)
    testBase64( mLog );
#endif
    return LRESULT();
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
