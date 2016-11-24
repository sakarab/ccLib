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
}
