#pragma once

#ifndef CCLIB_WIN_UTILS_H
#define CCLIB_WIN_UTILS_H

#include <string>
#include "cc_memory.hpp"
#include "LibLoader.h"

namespace ccwin
{

    /************************************************************
    ********    File Name Functions
    ***********************************************************/
    std::wstring IncludeTrailingPathDelimiter( const std::wstring& str );
    std::wstring ExcludeTrailingPathDelimiter( const std::wstring& str );
    std::wstring ExtractFileDrive( const std::wstring& fname );
    std::wstring ExtractFilePath( const std::wstring& fname );
    std::wstring ExtractFileName( const std::wstring& fname );
    std::wstring BaseFileName( const std::wstring& fname );
    std::wstring ChangeFileExt( const std::wstring& fname, const std::wstring& ext );

    /************************************************************
    ********    File Manipulation Functions
    ***********************************************************/
    bool FileExists( const std::wstring& fname );
    bool DirectoryExists( const std::wstring& directory );
    bool ForceDirectories( const std::wstring& dir );

#pragma region Common Directories
    /************************************************************
    ********    TGetDirAbstract
    ************************************************************/
    class TGetDirAbstract
    {
    public:
#if defined (UNICODE) || defined(_UNICODE)
        static const int    SHGetFolderPath_ERROR_CODE = E_FAIL;
#else
        static const int    SHGetFolderPath_ERROR_CODE = S_FALSE;
#endif
    private:
        virtual std::wstring get_AppDataDirectory_UserRoaming() = 0;
        virtual std::wstring get_AppDataDirectory_UserLocal() = 0;
        virtual std::wstring get_AppDataDirectory_System() = 0;
        virtual std::wstring get_PublicDocumentsDirectory() = 0;
    public:
        virtual ~TGetDirAbstract()                      {}
        std::wstring AppDataDirectory_UserRoaming()     { return get_AppDataDirectory_UserRoaming(); }
        std::wstring AppDataDirectory_UserLocal()       { return get_AppDataDirectory_UserLocal(); }
        std::wstring AppDataDirectory_System()          { return get_AppDataDirectory_System(); }
        std::wstring PublicDocumentsDirectory()         { return get_PublicDocumentsDirectory(); }
    };

    /************************************************************
    ********    TCommonDirectories
    ************************************************************/
    class TCommonDirectories
    {
    private:
        cclib::shared_ptr<TGetDirAbstract>      mDirectoryPrivider;
    public:
        TCommonDirectories();
        ~TCommonDirectories();

        std::wstring GetUserRoamingAppDataDirectory() { return mDirectoryPrivider->AppDataDirectory_UserRoaming(); }
        std::wstring GetUserLocalAppDataDirectory() { return mDirectoryPrivider->AppDataDirectory_UserLocal(); }
        std::wstring GetSystemAppDataDirectory() { return mDirectoryPrivider->AppDataDirectory_System(); }
        std::wstring GetPublicDocumentDirectory() { return mDirectoryPrivider->PublicDocumentsDirectory(); }
    };

    /************************************************************
    ********    TGetDirWin2K
    ************************************************************/
    class TGetDirWin2K : public TGetDirAbstract
    {
    private:
        std::wstring getCommonDirectory( int csidl );

        virtual std::wstring get_AppDataDirectory_UserRoaming() CC_OVERRIDE;
        virtual std::wstring get_AppDataDirectory_UserLocal() CC_OVERRIDE;
        virtual std::wstring get_AppDataDirectory_System() CC_OVERRIDE;
        virtual std::wstring get_PublicDocumentsDirectory() CC_OVERRIDE;
    };

#if ( _WIN32_WINNT >=  0x0600 )
    /************************************************************
    ********    TGetDirVista
    ************************************************************/
    class TGetDirVista : public TGetDirAbstract
    {
    private:
        typedef HRESULT( STDAPICALLTYPE *SHGetKnownFolderPath_Func )(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);
    private:
        LibLoader                   mShellDll;
        SHGetKnownFolderPath_Func   mSHGetKnownFolderPath;

        std::wstring getCommonDirectory( const KNOWNFOLDERID& folder_id );

        virtual std::wstring get_AppDataDirectory_UserRoaming() CC_OVERRIDE;
        virtual std::wstring get_AppDataDirectory_UserLocal() CC_OVERRIDE;
        virtual std::wstring get_AppDataDirectory_System() CC_OVERRIDE;
        virtual std::wstring get_PublicDocumentsDirectory() CC_OVERRIDE;
        // noncopyable
        TGetDirVista( const TGetDirVista& ) CC_EQ_DELETE;
        TGetDirVista& operator = ( const TGetDirVista& ) CC_EQ_DELETE;
    public:
        TGetDirVista();
        ~TGetDirVista();
    };
#endif

#pragma endregion

}

#endif
