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

#ifndef CCLIB_WIN_OS_UTILS_H
#define CCLIB_WIN_OS_UTILS_H

#include <Windows.h>
#include <string>

namespace ccwin
{
    /************************************************************
    ********    System Functions
    ************************************************************/
    bool RunningOnWOW64();
    bool IsWin64();

#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
    // these function must be statically linked to have the desired results
    HMODULE get_module_handle();
    std::wstring get_module_filename();
#endif

    std::wstring get_module_filename( HMODULE h_module );

    /************************************************************
    ********    WinOSVersion
    ************************************************************/
    class WinOSVersion
    {
    private:
        static OSVERSIONINFO InitValue();
        WinOSVersion();
    public:
        static const OSVERSIONINFO& Value();
        static int Win32Platform();
        static int Win32MajorVersion();
        static int Win32MinorVersion();
        static bool IsWindows7();
        static bool IsVista();
        static bool IsVistaGreater();
        static bool IsWinXP();
        static bool IsWin2K();
    };

    /************************************************************
    ********    OS Functions
    ***********************************************************/
    void RaiseOSError( DWORD last_error );
    void RaiseOSError( DWORD last_error, const char *message );
    void RaiseLastOSError();
    void RaiseLastOSError( const char *message );
    std::string SysErrorMessage( DWORD error_code );

    inline void RaiseOSError( DWORD last_error, const std::string& message )    { RaiseOSError( last_error, message.c_str() ); }
    inline void RaiseLastOSError( const std::string& message )                  { RaiseLastOSError( message.c_str() ); }

    std::wstring getAPPName();

    void ExecuteProgram( const std::wstring& command );
    DWORD ExecuteProgramWait( const std::wstring& command, DWORD timeout_msecs );
    bool IsUserAdmin();
}

#endif
