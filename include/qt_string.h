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

#ifndef CCLIB_QT_STRING_H
#define CCLIB_QT_STRING_H

#include "predef_cc.h"
#include "cpp_string.h"
#include <QString>

namespace ccqt
{
    std_string to_std_string( const QString& sstr );
    std::string narrow_string( const QString& sstr );
    std::wstring wide_string( const QString& sstr );

    inline QString fromStdString( const std::string& str )      { return QString::fromStdString( str ); }
    inline QString fromStdString( const std::wstring& str )     { return QString::fromStdWString( str ); }
}

#endif // QT_STRING_H
