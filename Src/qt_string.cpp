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

#include "pre_ccqt.h"
#include "qt_string.h"

namespace ccqt
{
    std::string narrow_string( const QString& sstr )
    {
        // uses toUtf8() according to documentation
        return sstr.toStdString();
    }

    std::wstring wide_string( const QString& sstr )
    {
        return sstr.toStdWString();
    }

    std_string to_std_string( const QString& sstr )
    {
#if defined(BOOST_OS_LINUX_AVAILABLE)
        return narrow_string( sstr );
#elif defined (BOOST_OS_WINDOWS_AVAILABLE)
        return wide_string( sstr );
#endif
    }
}
