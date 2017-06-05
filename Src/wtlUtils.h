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
        std::wstring    mSection;
        std::wstring    mKeyPrefix;
        int             mClientTop;
        int             mClientLeft;
        int             mClientWidth;
        int             mClientHeight;
        UINT            mShowState;

        UINT GetShowState( T *pT )
        {
            WINDOWPLACEMENT     wp;

            wp.length = sizeof( WINDOWPLACEMENT );
            GetWindowPlacement( *pT, &wp );
            return wp.showCmd;
        }

        LRESULT OnSize( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
        {
            if ( wParam == SIZE_RESTORED )
            {
                mClientWidth = LOWORD( lParam );
                mClientHeight = HIWORD( lParam );
                mShowState = wParam;
            }
            else if ( wParam == SIZE_MINIMIZED || wParam == SIZE_MAXIMIZED )
                mShowState = wParam;
            bHandled = FALSE;
            return 1;
        }

        LRESULT OnMove( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled )
        {
            T       *pT = static_cast<T*>(this);

            if ( GetShowState( pT ) == SW_SHOWNORMAL )
            {
                mClientLeft = LOWORD( lParam );
                mClientTop = HIWORD( lParam );
            }
            bHandled = FALSE;
            return 1;
        }

        BEGIN_MSG_MAP( CFormSize )
            MESSAGE_HANDLER( WM_SIZE, OnSize );
            MESSAGE_HANDLER( WM_MOVE, OnMove );
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
            mShowState = ini.ReadInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"ShowState" ).c_str(), SIZE_RESTORED );

            if ( mShowState == SIZE_RESTORED )
                pT->MoveWindow( mClientLeft, mClientTop, mClientWidth, mClientHeight, TRUE );
            else if ( mShowState == SIZE_MINIMIZED )
            {
                pT->ShowWindow( SW_MINIMIZE );
                pT->ShowWindow( SW_MINIMIZE );
            }
            else if ( mShowState == SIZE_MAXIMIZED )
            {
                pT->ShowWindow( SW_MAXIMIZE );
                pT->ShowWindow( SW_MAXIMIZE );
            }
        }

        void Save( ccwin::TIniFile& ini )
        {
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Left" ).c_str(), mClientLeft );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Top" ).c_str(), mClientTop );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Width" ).c_str(), mClientWidth );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"Height" ).c_str(), mClientHeight );
            ini.WriteInteger( mSection.c_str(), std::wstring( mKeyPrefix ).append( L"ShowState" ).c_str(), mShowState );
        }
    };

}

#endif
