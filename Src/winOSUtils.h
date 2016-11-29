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
    void RaiseLastOSError( DWORD last_error );
    void RaiseLastOSError();
    std::string SysErrorMessage( DWORD error_code );

    std::wstring getAPPName();

    void ExecuteProgram( const std::wstring& command );
    DWORD ExecuteProgramWait( const std::wstring& command, DWORD timeout_msecs );

}

#endif
