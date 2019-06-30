//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#pragma once

#if ! defined( CC_WTL_UTILS_H )
#define CC_WTL_UTILS_H

#include <string>
#include "cpp_string.h"
#include <atlctrls.h>
#include <ShellAPI.h>
#include "winClasses.h"

#define WTL_CLASS_USER                  WM_USER + 1000

#define WTLU_RECORD_WINDOW_SIZE         WTL_CLASS_USER + 1
#define WTLU_SET_WINDOW_SIZE            WTL_CLASS_USER + 2

namespace ccwtl
{
    //===========================================================
    //======    Resource Helper Functions
    //===========================================================
    std::wstring    ResourceString( UINT uID );
    const wchar_t * ResourceStringPtr( UINT uID );
    const wchar_t * ResourceStringPtr( UINT uID, int& len );

    //=======================================================================
    //==============    getControlText<T>
    //=======================================================================
    template <class CTRL> std_string getControlText( CTRL& ctrl )
    {
        int                     len = ctrl.GetWindowTextLength() + 1;
        std::vector<std_char>   result( len );

        ctrl.GetWindowText( &result.front(), len );
        return std_string( &result.front() );
    }

    std::wstring Get_Text( CComboBox& cb );
    std::wstring Get_Text( CListBox& cb );

    bool Menu_ToggleChecked( CMenu& menu, int menu_id );
    bool Menu_GetChecked( CMenu& menu, int menu_id );
    void Menu_SetCheck( CMenu& menu, int menu_id, bool value );
    bool Menu_ToggleChecked( CMenuHandle& menu, int menu_id );
    bool Menu_GetChecked( CMenuHandle& menu, int menu_id );
    void Menu_SetCheck( CMenuHandle& menu, int menu_id, bool value );

    //=======================================================================
    //==============    CFormSize<T>
    //=======================================================================
    template <class T> class CFormSize
    {
    private:
#if defined (CC_HAVE_ENUM_CLASS)
        enum class WindowState { Normal, Minimized, Maximized };
#else
        struct WindowState
        {
            static const int Normal = 0;
            static const int Minimized = 1;
            static const int Maximized  = 2;
        };
#endif
    private:
        std::wstring    mSection;
        std::wstring    mKeyPrefix;
        int             mClientTop;
        int             mClientLeft;
        int             mClientWidth;
        int             mClientHeight;
        WindowState     mWindowState;
        bool            mSizeSet;

        WindowState GetWindowState_2( T *pT )
        {
            if ( IsIconic( *pT ) )
                return WindowState::Minimized;
            else if ( IsZoomed( *pT ) )
                return WindowState::Maximized;
            return WindowState::Normal;
        }

        LRESULT OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
        {
            T       *pT = static_cast<T*>(this);

            pT->PostMessage( WTLU_RECORD_WINDOW_SIZE, 0, 0 );
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
        {
            T       *pT = static_cast<T*>(this);

            pT->PostMessage( WTLU_RECORD_WINDOW_SIZE, 0, 0 );
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnRecordSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
        {
            if ( mSizeSet )
            {
                T       *pT = static_cast<T*>(this);

                mWindowState = GetWindowState_2( pT );
                if ( mWindowState == WindowState::Normal )
                {
                    CRect   rect;

                    pT->GetWindowRect( &rect );
                    mClientLeft = rect.left;
                    mClientTop = rect.top;
                    mClientWidth = rect.Width();
                    mClientHeight = rect.Height();
                }
            }
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnSetSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
        {
            T       *pT = static_cast<T*>(this);

            ShowWindow( *pT, static_cast<int>(mWindowState) + 1 );
            mSizeSet = true;
            bHandled = FALSE;
            return 1;
        }

        BEGIN_MSG_MAP( CFormSize )
            MESSAGE_HANDLER( WM_SIZE, OnSize );
            MESSAGE_HANDLER( WM_MOVE, OnMove );
            MESSAGE_HANDLER( WTLU_RECORD_WINDOW_SIZE, OnRecordSize );
            MESSAGE_HANDLER( WTLU_SET_WINDOW_SIZE, OnSetSize );
        END_MSG_MAP()
    public:
        CFormSize( const std::wstring& section )
            : mSection( section ),
              mClientTop(), mClientLeft(), mClientWidth(), mClientHeight(),
              mWindowState(), mSizeSet(false)
        {}

        CFormSize( const std::wstring& section, const std::wstring& key_prefix )
            : mSection( section ), mKeyPrefix( key_prefix ),
              mClientTop(), mClientLeft(), mClientWidth(), mClientHeight(),
              mWindowState(), mSizeSet(false)
        {}

        void Load( ccwin::TIniFile& ini )
        {
            T       *pT = static_cast<T*>(this);
            CRect   rect;

            pT->GetWindowRect( &rect );
            mClientLeft = ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Left" ).c_str(), rect.left );
            mClientTop = ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Top" ).c_str(), rect.top );
            mClientWidth = ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Width" ).c_str(), rect.Width() );
            mClientHeight = ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Height" ).c_str(), rect.Height() );
            mWindowState = static_cast<WindowState>(ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"ShowState" ).c_str(),
                                                                     static_cast<int>(WindowState::Normal) ));
            if ( GetWindowState_2( pT ) == WindowState::Normal )
                SetWindowPos( *pT, 0, mClientLeft, mClientTop, mClientWidth, mClientHeight, SWP_NOZORDER | SWP_NOACTIVATE );
            else
            {
                WINDOWPLACEMENT     wp;

                wp.length = sizeof( WINDOWPLACEMENT );
                GetWindowPlacement( *pT, &wp );
                wp.rcNormalPosition = CRect( CPoint( mClientLeft, mClientTop ), CSize( mClientWidth, mClientHeight ) );
                SetWindowPlacement( *pT, &wp );
            }
            pT->PostMessage( WTLU_SET_WINDOW_SIZE, 0, 0 );
        }

        void Save( ccwin::TIniFile& ini )
        {
            if ( mSizeSet )
            {
                ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Left" ).c_str(), mClientLeft );
                ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Top" ).c_str(), mClientTop );
                ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Width" ).c_str(), mClientWidth );
                ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Height" ).c_str(), mClientHeight );
                ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"ShowState" ).c_str(), static_cast<int>(mWindowState) );
            }
        }
    };

    //=======================================================================
    //==============    CHDrop
    //=======================================================================
    template <class CHAR> class CHDrop
    {
        HDROP   m_hd;
        bool    bHandled;               // Checks if resources should be released.
        CHAR    m_buff[MAX_PATH + 5];   // DragQueryFile() wants LPTSTR.
    public:
        CHDrop( WPARAM wParam )
            : m_hd( reinterpret_cast<HDROP>(wParam) ), bHandled(false)
        {
            m_buff[0] = '\0';
        }

        ~CHDrop()
        {
            if ( bHandled )
                ::DragFinish( m_hd );
        }

        // Helper function, detects if the message is meant for 'this' window.
        BOOL IsInClientRect( HWND hw )
        {
            ATLASSERT( ::IsWindow( hw ) );

            POINT       p;

            ::DragQueryPoint( m_hd, &p );

            RECT        rc;

            ::GetClientRect( hw, &rc );

            return ::PtInRect( &rc, p );
        }

        // This function returns the number of files dropped on the window by the current operation.
        UINT GetNumFiles( void )
        {
            return ::DragQueryFile( m_hd, 0xffffFFFF, NULL, 0 );
        }

        // This function gets the whole file path for a file, given its ordinal number.
        UINT GetDropedFile( UINT iFile )
        {
            bHandled = true;
            return ::DragQueryFile( m_hd, iFile, m_buff, MAX_PATH );
        }

        std_string GetDropedFileName()              { return std_string( m_buff ); }

#ifdef _WTL_USE_CSTRING
        // CString overload for DragQueryFile (not used here, might be used by a handler
        // which is implemented outside CDropFilesHandler<T>.
        UINT GetDropedFile( UINT iFile, CString &cs )
        {
            bHandled = true;

            UINT    ret = ::DragQueryFile( m_hd, iFile, m_buff, MAX_PATH );

            cs = m_buff;
            return ret;
        }

        //inline operator CString() const         { return CString( m_buff ); }
#endif
        // Other string overloads (such as std::string) might come handy...

        // This class can 'be' the currently held file's path.
        //inline operator const CHAR *() const    { return m_buff; }
    }; // class CHDrop

}

#endif
