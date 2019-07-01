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

#ifndef CCLIB_WIN_UTILS_H
#define CCLIB_WIN_UTILS_H

#include <string>
#include "cc_memory.hpp"
#include "LibLoader.h"

#if ( _WIN32_WINNT >=  0x0600 )
#include <shtypes.h>
#endif

namespace ccwin
{
    //===========================================================
    //======    File Name Functions
    //===========================================================
    std::wstring IncludeTrailingPathDelimiter( const std::wstring& str );
    std::wstring ExcludeTrailingPathDelimiter( const std::wstring& str );
    std::wstring ExtractFileDrive( const std::wstring& fname );
    std::wstring ExtractFilePath( const std::wstring& fname );
    std::wstring ExtractFileName( const std::wstring& fname );
    std::wstring ExtractFileExt( const std::wstring& fname );
    std::wstring BaseFileName( const std::wstring& fname );
    std::wstring ChangeFileExt( const std::wstring& fname, const std::wstring& ext );

    //===========================================================
    //======    File Manipulation Functions
    //===========================================================
    bool FileExists( const std::wstring& fname );
    bool DirectoryExists( const std::wstring& directory );
    inline bool IsFile( const std::wstring& fname )             { return FileExists( fname ); }
    inline bool IsDirectory( const std::wstring& directory )    { return DirectoryExists( directory ); }
    bool ForceDirectories( const std::wstring& dir );

    //===========================================================
    //======    Resource Helper Functions
    //===========================================================
    std::wstring ResourceString( HINSTANCE hInstance, UINT uID );
    const wchar_t *ResourceStringPtr( HINSTANCE hInstance, UINT uID, int& len );

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

        std::wstring AppDataDirectory_UserRoaming()     { return mDirectoryPrivider->AppDataDirectory_UserRoaming(); }
        std::wstring AppDataDirectory_UserLocal()       { return mDirectoryPrivider->AppDataDirectory_UserLocal(); }
        std::wstring AppDataDirectory_System()          { return mDirectoryPrivider->AppDataDirectory_System(); }
        std::wstring PublicDocumentsDirectory()         { return mDirectoryPrivider->PublicDocumentsDirectory(); }
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
