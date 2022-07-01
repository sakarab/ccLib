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
    namespace FilesDlg
    {
        constexpr DWORD FileMustExist = OFN_FILEMUSTEXIST;
        constexpr DWORD PathMustExist = OFN_PATHMUSTEXIST;
        constexpr DWORD AllowMultiSelect = OFN_ALLOWMULTISELECT;

        typedef std::pair<LPCTSTR, LPCTSTR>             filter_spec;
        typedef std::initializer_list<filter_spec>      filter_list;

        struct Result
        {
            bool            OK;
            std::wstring    FileName;
        };

        Result Open( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const filter_list& filters, HWND wnd );
        Result Save( const std::wstring& def_ext, const std::wstring& filename, DWORD flags, const filter_list& filters, HWND wnd );
        Result SelectFolder( const std::wstring& def_path, HWND wnd );
    }
}

#endif
