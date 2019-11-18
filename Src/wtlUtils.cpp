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
        bool Menu_ToggleChecked( HMENU menu, UINT menu_id )
        {
            MENUITEMINFO    mii;

            mii.cbSize = sizeof( MENUITEMINFO );
            mii.fMask = MIIM_STATE;
            ::GetMenuItemInfo( menu, menu_id, FALSE, &mii );
            mii.fState ^= MFS_CHECKED;
            ::SetMenuItemInfo( menu, menu_id, FALSE, &mii );
            return (mii.fState & MFS_CHECKED) != 0;
        }

        bool Menu_GetChecked( HMENU menu, UINT menu_id )
        {
            MENUITEMINFO    mii;

            mii.cbSize = sizeof( MENUITEMINFO );
            mii.fMask = MIIM_STATE;
            ::GetMenuItemInfo( menu, menu_id, FALSE, &mii );
            return (mii.fState & MFS_CHECKED) != 0;
        }

        void Menu_SetCheck( HMENU menu, UINT menu_id, bool value )
        {
            MENUITEMINFO    mii;

            mii.cbSize = sizeof( MENUITEMINFO );
            mii.fMask = MIIM_STATE;
            ::GetMenuItemInfo( menu, menu_id, FALSE, &mii );
            if ( value )
                mii.fState |= MFS_CHECKED;
            else
                mii.fState &= ~MFS_CHECKED;
            ::SetMenuItemInfo( menu, menu_id, FALSE, &mii );
        }

        Menu_ItemIndex Menu_HandleFromID( CMenuHandle menu, UINT menu_id )
        {
            int             menu_count = menu.GetMenuItemCount();

            for ( int n = 0 ; n < menu_count ; ++n )
            {
                CString         sstr;
                menu.GetMenuString( n, sstr, MF_BYPOSITION );

                if ( menu.GetMenuItemID( n ) == menu_id )
#if defined (CC_HAVE_UNIFORM_INIT)
                    return { menu, n };
#else
                    return Menu_ItemIndex( menu, n );
#endif

                MENUITEMINFO    mii;

                mii.cbSize = sizeof(MENUITEMINFO);
                mii.fMask = MIIM_SUBMENU;
                menu.GetMenuItemInfo( n, TRUE, &mii );
                if ( mii.hSubMenu )
                {
#if defined (CC_HAVE_IF_WITH_INIT)
                    if ( Menu_ItemIndex result = Menu_HandleFromID( mii.hSubMenu, menu_id ) ; result.Index >= 0 )
#else
                    Menu_ItemIndex  result = Menu_HandleFromID( mii.hSubMenu, menu_id );
                    if ( result.Index >= 0 )
#endif
                        return result;
                }
            }
#if defined (CC_HAVE_UNIFORM_INIT)
            return { 0, -1 };
#else
            return Menu_ItemIndex( 0, -1 );
#endif
        }

    }

    bool Menu_ToggleChecked( CMenu& menu, UINT menu_id )                        { return Menu_ToggleChecked( menu.m_hMenu, menu_id ); }
    bool Menu_GetChecked( CMenu& menu, UINT menu_id )                           { return Menu_GetChecked( menu.m_hMenu, menu_id ); }
    void Menu_SetCheck( CMenu& menu, UINT menu_id, bool value )                 { Menu_SetCheck( menu.m_hMenu, menu_id, value ); }
    bool Menu_ToggleChecked( CMenuHandle& menu, UINT menu_id )                  { return Menu_ToggleChecked( menu.m_hMenu, menu_id ); }
    bool Menu_GetChecked( CMenuHandle& menu, UINT menu_id )                     { return Menu_GetChecked( menu.m_hMenu, menu_id ); }
    void Menu_SetCheck( CMenuHandle& menu, UINT menu_id, bool value )           { Menu_SetCheck( menu.m_hMenu, menu_id, value ); }
    Menu_ItemIndex Menu_HandleFromID( CMenu& menu, UINT menu_id )               { return Menu_HandleFromID( menu.m_hMenu, menu_id ); }
}
