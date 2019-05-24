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

#include "pre_ccwtl.h"
#include "wtlUtils.h"
#include <atlapp.h>

namespace ccwtl
{
    //===========================================================
    //======    Resource Helper Functions
    //===========================================================
    std::wstring ResourceString( UINT uID )
    {
        int             len = 0;
        const wchar_t * buffer = ResourceStringPtr( uID, len );

        return std::wstring( buffer, len );
    }

    const wchar_t * ResourceStringPtr( UINT uID )
    {
        return WTL::AtlGetStringPtr( uID, NULL );
    }

    const wchar_t * ResourceStringPtr( UINT uID, int& len )
    {
        return WTL::AtlGetStringPtr( uID, &len );
    }

    //=======================================================================
    //==============    getControlText<T>
    //=======================================================================
    std::wstring Get_Text( CComboBox& cb )
    {
        int         idx = cb.GetCurSel();

        if ( idx >= 0 )
        {
            CString     str;

            cb.GetLBText( idx, str );
            return std::wstring( str );
        }
        return std::wstring();
    }

    std::wstring Get_Text( CListBox& cb )
    {
        int         idx = cb.GetCurSel();

        if ( idx >= 0 )
        {
            CString     str;

            cb.GetText( idx, str );
            return std::wstring( str );
        }
        return std::wstring();
    }

    namespace
    {
        bool Menu_ToggleChecked( HMENU menu, int menu_id )
        {
            MENUITEMINFO    mii;

            mii.cbSize = sizeof( MENUITEMINFO );
            mii.fMask = MIIM_STATE;
            ::GetMenuItemInfo( menu, menu_id, FALSE, &mii );
            mii.fState ^= MFS_CHECKED;
            ::SetMenuItemInfo( menu, menu_id, FALSE, &mii );
            return (mii.fState & MFS_CHECKED) != 0;
        }

        bool Menu_GetChecked( HMENU menu, int menu_id )
        {
            MENUITEMINFO    mii;

            mii.cbSize = sizeof( MENUITEMINFO );
            mii.fMask = MIIM_STATE;
            ::GetMenuItemInfo( menu, menu_id, FALSE, &mii );
            return (mii.fState & MFS_CHECKED) != 0;
        }
    }


    bool Menu_ToggleChecked( CMenu& menu, int menu_id )
    {
        return Menu_ToggleChecked( menu.m_hMenu, menu_id );
    }

    bool Menu_GetChecked( CMenu& menu, int menu_id )
    {
        return Menu_GetChecked( menu.m_hMenu, menu_id );
    }

    bool Menu_ToggleChecked( CMenuHandle& menu, int menu_id )
    {
        return Menu_ToggleChecked( menu.m_hMenu, menu_id );
    }

    bool Menu_GetChecked( CMenuHandle& menu, int menu_id )
    {
        return Menu_GetChecked( menu.m_hMenu, menu_id );
    }
}
