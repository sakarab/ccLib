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
#include "winClasses.h"

#define WM_USER_WTL             WM_USER + 1000

#define RECORD_WINDOW_SIZE      WM_USER_WTL + 1

namespace ccwtl
{
    template <class CTRL> std_string getControlText( CTRL& ctrl )
    {
        int                     len = ctrl.GetWindowTextLength() + 1;
        std::vector<std_char>   result( len );

        ctrl.GetWindowText( &result.front(), len );
        return std_string( &result.front() );
    }

    std::wstring Get_Text( CComboBox& cb );

    bool Menu_ToggleChecked( CMenu& menu, int menu_id );
    bool Menu_GetChecked( CMenu& menu, int menu_id );

    //=======================================================================
    //==============    CFormSize<T>
    //=======================================================================
    template <class T> class CFormSize
    {
    private:
        enum class WindowState { Normal, Minimized, Maximized };
    private:
        std::wstring    mSection;
        std::wstring    mKeyPrefix;
        int             mClientTop;
        int             mClientLeft;
        int             mClientWidth;
        int             mClientHeight;
        WindowState     mWindowState;

        WindowState GetWindowState( T *pT )
        {
            WINDOWPLACEMENT     wp;

            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( *pT, &wp );
            if ( wp.showCmd == SW_SHOWMINIMIZED )
                return WindowState::Minimized;
            else if ( wp.showCmd == SW_SHOWMAXIMIZED )
                return WindowState::Maximized;
            return WindowState::Normal;
        }

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

            pT->PostMessage( RECORD_WINDOW_SIZE, 0, 0 );
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
        {
            T       *pT = static_cast<T*>(this);

            pT->PostMessage( RECORD_WINDOW_SIZE, 0, 0 );
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnRecordSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
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
            bHandled = FALSE;
            return 1;
        }

        BEGIN_MSG_MAP( CFormSize )
            MESSAGE_HANDLER( WM_SIZE, OnSize );
            MESSAGE_HANDLER( WM_MOVE, OnMove );
            MESSAGE_HANDLER( RECORD_WINDOW_SIZE, OnRecordSize );
        END_MSG_MAP()
    public:
        CFormSize( const std::wstring& section )
            : mSection( section )
        {}

        CFormSize( const std::wstring& section, const std::wstring& key_prefix )
            : mSection( section ), mKeyPrefix( key_prefix )
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
            ShowWindow( *pT, static_cast<int>(mWindowState) + 1 );
        }

        void Save( ccwin::TIniFile& ini )
        {
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Left" ).c_str(), mClientLeft );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Top" ).c_str(), mClientTop );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Width" ).c_str(), mClientWidth );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Height" ).c_str(), mClientHeight );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"ShowState" ).c_str(), static_cast<int>(mWindowState) );
        }
    };

}

#endif
