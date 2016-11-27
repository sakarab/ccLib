#pragma once

#ifndef CCLIB_WIN_UTILS_H
#define CCLIB_WIN_UTILS_H

#include <string>

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

}

#endif
