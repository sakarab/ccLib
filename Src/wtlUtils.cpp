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

#include <pre_cc.h>
#include "wtlUtils.h"

namespace ccwtl
{
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

    bool Menu_ToggleChecked( CMenu& menu, int menu_id )
    {
        MENUITEMINFO    mii;

        mii.cbSize = sizeof( MENUITEMINFO );
        mii.fMask = MIIM_STATE;
        menu.GetMenuItemInfo( menu_id, FALSE, &mii );
        mii.fState ^= MFS_CHECKED;
        menu.SetMenuItemInfo( menu_id, FALSE, &mii );
        return (mii.fState & MFS_CHECKED) != 0;
    }

    bool Menu_GetChecked( CMenu& menu, int menu_id )
    {
        MENUITEMINFO    mii;

        mii.cbSize = sizeof( MENUITEMINFO );
        mii.fMask = MIIM_STATE;
        menu.GetMenuItemInfo( menu_id, FALSE, &mii );
        return (mii.fState & MFS_CHECKED) != 0;
    }

}
