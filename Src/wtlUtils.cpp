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
