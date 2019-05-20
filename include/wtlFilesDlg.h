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

#if ! defined( CC_WTL_FILES_DLG_H )
#define CC_WTL_FILES_DLG_H

#include <vector>
#include <string>
#include <atldlgs.h>
#include <cpp_lpstr.h>
#include <winUtils.h>

namespace ccwtl
{
    typedef std::pair<LPCTSTR, LPCTSTR>                 open_filter_spec;
    typedef std::initializer_list<open_filter_spec>     open_filter_list;

    struct FilesDlgResult
    {
        bool            OK;
        std::wstring    FileName;
    };

    //=======================================================================
    //======    FilesDlg
    //=======================================================================
    class FilesDlg
    {
#if WINVER < 0x0600
    private:
        std::wstring MakeFilterString_XP( const open_filter_list& filters )
        {
            std::wstring    result;

            for ( const open_filter_spec& filter : filters )
            {
                result.append( filter.first );
                result.append( 1, '\0' );
                result.append( filter.second );
                result.append( 1, '\0' );
            }
            result.append( 1, '\0' );
            return result;
        }

        FilesDlgResult FileDlg_XP( BOOL is_open, const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const open_filter_list& filters, HWND wnd )
        {
            auto            fname = cclib::LPSTR( filename );
            auto            filter = cclib::LPSTR( MakeFilterString_XP( filters ) );
            CFileDialog     dlg( is_open,                                               // TRUE for FileOpen, FALSE for FileSaveAs
                                    def_ext.empty() ? nullptr : def_ext.c_str(),           // LPCTSTR lpszDefExt = 
                                    fname.get(),                                           // LPCTSTR lpszFileName = 
                                    flags,                                                 // DWORD dwFlags = 
                                    filter.get(),                                          // LPCTSTR lpszFilter =
                                    wnd );

            FilesDlgResult      result;

            result.OK = dlg.DoModal( wnd ) == IDOK;
            if ( result.OK )
                result.FileName = std::wstring( dlg.m_szFileName );
            return result;
        }
#else
    private:
        typedef std::vector<COMDLG_FILTERSPEC>      FILTERSPEC_array;

        FILTERSPEC_array MakeFilterString_7( const open_filter_list& filters );

        template<typename DLG> FilesDlgResult FileDlg_7( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const open_filter_list& filters, HWND wnd )
        {
            auto                    fname = cclib::LPSTR( ccwin::ExtractFileName( filename ) );
            FILTERSPEC_array        fspecs = MakeFilterString_7( filters );

            DLG     dlg( fname.get(),                                       // LPCTSTR lpszFileName
                         flags,
                         def_ext.empty() ? nullptr : def_ext.c_str(),       // LPCTSTR lpszDefExt = 
                         fspecs.empty() ? nullptr : &fspecs.front(),
                         filters.size() );

            FilesDlgResult      result;

            result.OK = dlg.DoModal( wnd ) == IDOK;
            if ( result.OK )
            {
                CString     sstr;

                dlg.GetFilePath( sstr );
                result.FileName = std::wstring( static_cast<LPCTSTR>(sstr) );

            }
            return result;
        }
#endif
    public:
        FilesDlgResult OpenDlg( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const open_filter_list& filters, HWND wnd )
        {
#if WINVER >= 0x0600
            return FileDlg_7<CShellFileOpenDialog>( def_ext, filename, flags, filters, wnd );
#else
            return FileDlg_XP( TRUE, def_ext, filename, flags, filters, wnd );
#endif
        }

        FilesDlgResult SaveDlg( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const open_filter_list& filters, HWND wnd )
        {
#if WINVER >= 0x0600
            return FileDlg_7<CShellFileSaveDialog>( def_ext, filename, flags, filters, wnd );
#else
            return FileDlg_XP( FALSE, def_ext, filename, flags, filters, wnd );
#endif
        }
    };
}

#endif
