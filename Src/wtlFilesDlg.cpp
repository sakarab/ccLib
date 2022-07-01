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

#include <atldlgs.h>
#include <cpp_lpstr.h>
#include <winUtils.h>
#include "wtlFilesDlg.h"

namespace ccwtl
{
    // most OFN_* / FOS_* constants are the same. Propably they won't change but we must be on the safe side
    static_assert( OFN_FILEMUSTEXIST == FOS_FILEMUSTEXIST, "wtlFilesDlg.h - OFN_* / FOS_* value mismatch" );
    static_assert( OFN_PATHMUSTEXIST == FOS_PATHMUSTEXIST, "wtlFilesDlg.h - OFN_* / FOS_* value mismatch" );
    static_assert( OFN_ALLOWMULTISELECT == FOS_ALLOWMULTISELECT, "wtlFilesDlg.h - OFN_* / FOS_* value mismatch" );

    namespace
    {
        typedef std::vector<COMDLG_FILTERSPEC>      FILTERSPEC_array;

        bool supports_vista_dlg()
        {
            ATL::CComPtr<IFileOpenDialog>       m_spFileDlg;
            HRESULT                             hRet = m_spFileDlg.CoCreateInstance(CLSID_FileOpenDialog);

            return (SUCCEEDED(hRet));
        }

        std::wstring MakeFilterString_XP( const FilesDlg::filter_list& filters )
        {
            std::wstring    result;

            for ( const FilesDlg::filter_spec& filter : filters )
            {
                result.append( filter.first );
                result.append( 1, '\0' );
                result.append( filter.second );
                result.append( 1, '\0' );
            }
            result.append( 1, '\0' );
            return result;
        }

        FILTERSPEC_array MakeFilterString_7( const FilesDlg::filter_list& filters )
        {
            FILTERSPEC_array    result;

            for ( auto& item : filters )
                result.push_back( {item.first, item.second} );
            return result;
        }

        FilesDlg::Result FileDlg_XP( BOOL is_open, const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const FilesDlg::filter_list& filters, HWND wnd )
        {
            flags |= (OFN_NOCHANGEDIR | OFN_ENABLESIZING | OFN_HIDEREADONLY);

            auto            fname = cclib::LPSTR( filename );
            auto            filter = cclib::LPSTR( MakeFilterString_XP( filters ) );
            CFileDialog     dlg( is_open,                                               // TRUE for FileOpen, FALSE for FileSaveAs
                                 def_ext.empty() ? nullptr : def_ext.c_str(),           // LPCTSTR lpszDefExt = 
                                 fname.get(),                                           // LPCTSTR lpszFileName = 
                                 flags,                                                 // DWORD dwFlags = 
                                 filter.get(),                                          // LPCTSTR lpszFilter =
                                 wnd );

            FilesDlg::Result     result;

            result.OK = dlg.DoModal( wnd ) == IDOK;
            if ( result.OK )
                result.FileName = std::wstring( dlg.m_szFileName );
            return result;
        }

        template<typename DLG> FilesDlg::Result FileDlg_7( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const FilesDlg::filter_list& filters, HWND wnd )
        {
            flags |= (FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM);

            auto                    fname = cclib::LPSTR( ccwin::ExtractFileName( filename ) );
            FILTERSPEC_array        fspecs = MakeFilterString_7( filters );
            DLG                     dlg( fname.get(),                                       // LPCTSTR lpszFileName
                                         flags,
                                         def_ext.empty() ? nullptr : def_ext.c_str(),       // LPCTSTR lpszDefExt = 
                                         fspecs.empty() ? nullptr : &fspecs.front(),
                                         filters.size() );
            FilesDlg::Result        result;

            result.OK = dlg.DoModal( wnd ) == IDOK;
            if ( result.OK )
            {
                CString     sstr;

                dlg.GetFilePath( sstr );
                result.FileName = std::wstring( static_cast<LPCTSTR>(sstr) );
            }
            return result;
        }

        FilesDlg::Result FolderDlg_7( const std::wstring& def_path, HWND wnd )
        {
            return FileDlg_7<CShellFileOpenDialog>( std_string(), def_path, FOS_PICKFOLDERS, {}, wnd );
        }

        FilesDlg::Result FolderDlg_XP( const std::wstring& def_path, HWND wnd )
        {
            CFolderDialog       dlg;
            FilesDlg::Result    result;
            auto                initial_path = cclib::LPSTR( def_path );

            dlg.SetInitialFolder( initial_path.get() );
            result.OK = dlg.DoModal( wnd ) == IDOK;
            if ( result.OK )
                result.FileName = std_string( dlg.GetFolderPath() );
            return result;
        }

    } // nemspace

    namespace FilesDlg
    {
        Result Open( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const filter_list& filters, HWND wnd )
        {
            if ( supports_vista_dlg() )
                return FileDlg_7<CShellFileOpenDialog>( def_ext, filename, flags, filters, wnd );
            return FileDlg_XP( TRUE, def_ext, filename, flags, filters, wnd );
        }
        
        Result Save( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const filter_list& filters, HWND wnd )
        {
            if ( supports_vista_dlg() )
                return FileDlg_7<CShellFileSaveDialog>( def_ext, filename, flags, filters, wnd );
            return FileDlg_XP( FALSE, def_ext, filename, flags, filters, wnd );
        }

        Result SelectFolder( const std::wstring& def_path, HWND wnd )
        {
            if ( supports_vista_dlg() )
                return FolderDlg_7( def_path, wnd );
            return FolderDlg_XP( def_path, wnd );
        }
    }
}
