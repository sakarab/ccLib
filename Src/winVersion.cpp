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

#include "winVersion.h"
#include "winOSUtils.h"

namespace ccwin
{
    VS_FIXEDFILEINFO GetFileVersionFixed( const std::wstring& filename )
    {
        VS_FIXEDFILEINFO    result;

        memset( &result, 0, sizeof( result ) );
        result.dwSignature = 0xFEEF04BD;

        DWORD               unused;
        DWORD               size = GetFileVersionInfoSize( filename.c_str(), &unused );

        if ( size == 0 )
        {
            result.dwSignature = GetLastError();
            return result;
        }

        boost::scoped_array<char>   Pt( new char[size] );

        GetFileVersionInfo( filename.c_str(), 0, size, Pt.get() );

        void    *Pt2;
        UINT    size3 = 0;

        if ( VerQueryValue( Pt.get(), TEXT( "\\" ), &Pt2, &size3 ) && Pt2 )
            result = *static_cast<VS_FIXEDFILEINFO *>(Pt2);
        return result;
    }

    VS_FIXEDFILEINFO GetFileVersionFixed( HMODULE h_module )
    {
        return GetFileVersionFixed( get_module_filename( h_module ) );
    }

    std::wstring GetFileVersionStr( const std::wstring& filename )
    {
        VS_FIXEDFILEINFO    fixed = GetFileVersionFixed( filename );

        int     hi_ms = HIWORD( fixed.dwFileVersionMS );
        int     lo_ms = LOWORD( fixed.dwFileVersionMS );
        int     hi_ls = HIWORD( fixed.dwFileVersionLS );
        int     lo_ls = LOWORD( fixed.dwFileVersionLS );

#if defined (CC_HAVE_FMT_FORMAT)
        return fmt::format( TEXT( "{1}.{2}.{3}.{4}" ), hi_ms % lo_ms % hi_ls % lo_ls );
#else
        return boost::str( boost::wformat( TEXT( "%1%.%2%.%3%.%4%" ) ) % hi_ms % lo_ms % hi_ls % lo_ls );
#endif
    }

    std::wstring GetFileVersionStr( HMODULE h_module )
    {
        return GetFileVersionStr( get_module_filename( h_module ) );
    }
}
