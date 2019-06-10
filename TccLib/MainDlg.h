// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <predef_cc.h>
#include <winBuffers.h>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
                 public CDialogResize<CMainDlg>,
                 public CMessageFilter, public CIdleHandler
{
private:
    CRichEditCtrl               mLog;
    ccwin::TRitchEditStreambuf  mLogger;

    virtual BOOL PreTranslateMessage(MSG* pMsg) CC_OVERRIDE;
    virtual BOOL OnIdle() CC_OVERRIDE;

    void CloseDialog(int nVal);

    // Handler prototypes (uncomment arguments if needed):
    //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnClearLog(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnTestStringList(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    BEGIN_MSG_MAP( CMainDlg )
        MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog );
        MESSAGE_HANDLER( WM_DESTROY, OnDestroy );
        COMMAND_ID_HANDLER( ID_APP_ABOUT, OnAppAbout );
        COMMAND_ID_HANDLER( IDOK, OnOK );
        COMMAND_ID_HANDLER( IDCANCEL, OnCancel );
        COMMAND_ID_HANDLER( ID_FILE_CLEARLOG, OnClearLog );
        COMMAND_ID_HANDLER( ID_TESTS_STRINGLIST, OnTestStringList );
        CHAIN_MSG_MAP( CDialogResize<CMainDlg> );
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP( CMainDlg )
        DLGRESIZE_CONTROL( IDC_RICHEDIT2_LOG, DLSZ_SIZE_X | DLSZ_SIZE_Y )
    END_DLGRESIZE_MAP()

public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()
};
