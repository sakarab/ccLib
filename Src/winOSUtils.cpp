#include <pre_cc.h>
#include "winOSUtils.h"
#include "smException.h"
#include "win_str.h"
#include "cc_array.hpp"
#include "cc_memory.hpp"
#include <boost/scope_exit.hpp>
#include <boost/format.hpp>

namespace
{
    bool RunningOnWOW64_Internal()
    {
        typedef BOOL( WINAPI *LPFN_ISWOW64PROCESS ) (HANDLE, PBOOL);

        BOOL                    bIsWow64 = FALSE;
        LPFN_ISWOW64PROCESS     fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>
                                                   (GetProcAddress( GetModuleHandle( TEXT( "kernel32" ) ), "IsWow64Process" ));

        if ( fnIsWow64Process )
            if ( !fnIsWow64Process( GetCurrentProcess(), &bIsWow64 ) )
                ccwin::RaiseLastOSError();
        return bIsWow64 != 0;
    }

    bool IsWin64_Internal()
    {
        typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

        // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
        SYSTEM_INFO     si;
        PGNSI           pGNSI = reinterpret_cast<PGNSI>
                                (GetProcAddress( GetModuleHandle( TEXT( "kernel32.dll" ) ), "GetNativeSystemInfo" ));

        ZeroMemory( &si, sizeof( si ) );
        if ( 0 != pGNSI )
            pGNSI( &si );
        else
            GetSystemInfo( &si );
        return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64;
    }

    DWORD DoExecuteProgramWait( const std::wstring& command, std_char *current_dir, HANDLE hOut, DWORD timeout_msecs )
    {
        STARTUPINFO     startup_info;
        BOOL            inherit_handles = FALSE;

        memset( &startup_info, 0, sizeof( startup_info ) );
        startup_info.cb = sizeof( startup_info );
        if ( hOut != 0 )
        {
            startup_info.hStdError = hOut;
            startup_info.hStdOutput = hOut;
            startup_info.dwFlags = STARTF_USESTDHANDLES;
            inherit_handles = TRUE;
        }

        PROCESS_INFORMATION     process_info;

        if ( !CreateProcess( NULL, ccwin::smLPSTR( command ).get(), NULL, NULL, inherit_handles, 0, NULL, current_dir, &startup_info, &process_info ) )
            ccwin::RaiseLastOSError();

        BOOST_SCOPE_EXIT( (&process_info) )
        {
            CloseHandle( process_info.hThread );
            CloseHandle( process_info.hProcess );
        }
        BOOST_SCOPE_EXIT_END;

        DWORD       ret = WaitForSingleObject( process_info.hProcess, timeout_msecs );

        if ( ret == WAIT_FAILED )
            ccwin::RaiseLastOSError();
        else if ( ret == WAIT_TIMEOUT )
            throw cclib::BaseException( "rsExecuteTimeOut" );

        DWORD       result;

        if ( !GetExitCodeProcess( process_info.hProcess, &result ) )
            ccwin::RaiseLastOSError();
        return result;
    }

    void DoExecuteProgram( const std::wstring& command, std_char *current_dir )
    {
        STARTUPINFO     startup_info;
        BOOL            inherit_handles = FALSE;

        memset( &startup_info, 0, sizeof( startup_info ) );
        startup_info.cb = sizeof( startup_info );

        PROCESS_INFORMATION     process_info;

        if ( !CreateProcess( NULL, ccwin::smLPSTR( command ).get(), NULL, NULL, inherit_handles, 0, NULL, current_dir, &startup_info, &process_info ) )
            ccwin::RaiseLastOSError();

        CloseHandle( process_info.hThread );
        CloseHandle( process_info.hProcess );
    }

    std::wstring get_module_filename( HMODULE h_module )
    {
        WCHAR       fname[MAX_PATH];
        DWORD       ret = GetModuleFileName( h_module, fname, MAX_PATH );
        DWORD       err = GetLastError();

        if ( ret == 0 || (ret == MAX_PATH && err == ERROR_INSUFFICIENT_BUFFER) )
            ccwin::RaiseLastOSError();
        return std::wstring( fname );
    }
}
// namespace

namespace ccwin
{

    /************************************************************
    ********    System Functions
    ************************************************************/
    bool RunningOnWOW64()
    {
        return RunningOnWOW64_Internal();
    }

    bool IsWin64()
    {
        return IsWin64_Internal();
    }

    /************************************************************
    ********    WinOSVersion
    ************************************************************/
    OSVERSIONINFO WinOSVersion::InitValue()
    {
        OSVERSIONINFO    result;

        result.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
        GetVersionEx( &result );
        return result;
    }

    const OSVERSIONINFO& WinOSVersion::Value()
    {
        static OSVERSIONINFO    version = InitValue();

        return version;
    }

    int WinOSVersion::Win32Platform()       { return Value().dwPlatformId; }
    int WinOSVersion::Win32MajorVersion()   { return Value().dwMajorVersion; }
    int WinOSVersion::Win32MinorVersion()   { return Value().dwMinorVersion; }

    bool WinOSVersion::IsWindows7()         { return Value().dwMajorVersion == 6 && Value().dwMinorVersion == 1; }
    bool WinOSVersion::IsVista()            { return Value().dwMajorVersion == 6 && Value().dwMinorVersion == 0; }
    bool WinOSVersion::IsVistaGreater()     { return Value().dwMajorVersion >= 6; }
    bool WinOSVersion::IsWinXP()            { return (Value().dwMajorVersion == 5 && Value().dwMinorVersion == 1) || (Value().dwMajorVersion == 5 && Value().dwMinorVersion == 2); }
    bool WinOSVersion::IsWin2K()            { return Value().dwMajorVersion == 5 && Value().dwMinorVersion == 0; }

    /************************************************************
    ********    OS Functions
    ***********************************************************/
    std::string SysErrorMessage( DWORD error_code )
    {
        cclib::array<char, 256>         buffer;

        int     len = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                                      0, error_code, 0, buffer.data(), size_cast<DWORD>(buffer.size()), 0 );

        while ( len > 0 )
        {
            char    ch = buffer[len - 1];

            if ( ch < 32 || ch == '.' )
                --len;
            else
                break;
        }
        return std::string( buffer.data(), len );
    }

    void RaiseLastOSError()
    {
        RaiseLastOSError( GetLastError() );
    }

    void RaiseLastOSError( DWORD last_error )
    {
        if ( last_error != 0 )
            throw cclib::BaseException( boost::str( boost::format( "%1%" ) % SysErrorMessage( last_error ) ) );
        throw cclib::BaseException( "Unknown OS Error." );
    }

    void ExecuteProgram( const std::wstring& command )
    {
        DoExecuteProgram( command, nullptr );
    }

    DWORD ExecuteProgramWait( const std::wstring& command, DWORD timeout_msecs )
    {
        return DoExecuteProgramWait( command, nullptr, nullptr, timeout_msecs );
    }

    std::wstring getAPPName()
    {
        return get_module_filename( 0 );
    }

}
