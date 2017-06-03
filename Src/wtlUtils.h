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
}

#endif