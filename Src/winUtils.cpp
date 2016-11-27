#include <pre_cc.h>
#include "winUtils.h"
#include "smException.h"

namespace
{
    const wchar_t DriveDelim = L':';
    const wchar_t PathDelim = L'\\';
    const wchar_t DotDelim = L'.';

    bool CreateDir( const std::wstring& dir )
    {
        return CreateDirectory( dir.c_str(), 0 ) != 0;
    }
}
// namespace

namespace ccwin
{

    /************************************************************
    ********    File Name Functions
    ***********************************************************/

    std::wstring ExtractFileDrive( const std::wstring& fname )
    {
        if ( fname.length() >= 2 && fname[1] == DriveDelim )
            return fname.substr( 0, 2 );
        else if ( fname.length() >= 2 && fname[0] == PathDelim && fname[1] == PathDelim )
        {
            std::wstring::size_type     J = 0;
            std::wstring::size_type     I = 2;

            while ( I < fname.length() && J < 2 )
            {
                if ( fname[I] == PathDelim )
                    ++J;
                if ( J < 2 )
                    ++I;
            }
            if ( I < fname.length() && fname[I] == PathDelim )
                --I;
            return fname.substr( 0, I );
        }
        else
            return std::wstring();
    }

    std::wstring ExtractFilePath( const std::wstring& fname )
    {
        std::wstring::size_type     pos = fname.find_last_of( PathDelim );

        if ( pos == std::wstring::npos )
            return std::wstring();
        return fname.substr( 0, pos + 1 );
    }

    std::wstring ExtractFileName( const std::wstring& fname )
    {
        std::wstring::size_type     pos = fname.find_last_of( PathDelim );

        return (pos != std::wstring::npos ? fname.substr( pos + 1 ) : fname);
    }

    std::wstring BaseFileName( const std::wstring& fname )
    {
        std::wstring                result = ExtractFileName( fname );
        std::wstring::size_type     pos = result.find_last_of( DotDelim );

        if ( pos != std::wstring::npos )
            result.resize( pos );
        return result;
    }

    std::wstring ChangeFileExt( const std::wstring& fname, const std::wstring& ext )
    {
        std::wstring::size_type   last_dot_pos = fname.rfind( DotDelim );

        if ( last_dot_pos == std::wstring::npos )
            return std::wstring( fname ).append( ext );
        return fname.substr( 0, last_dot_pos ).append( ext );
    }

    /************************************************************
    ********    File Manipulation Functions
    ***********************************************************/
    bool FileExists( const std::wstring& fname )
    {
        struct _stat    info;
        int             res = _tstat( fname.c_str(), &info );

        return res == 0;
    }

    bool DirectoryExists( const std::wstring& directory )
    {
        DWORD   code = GetFileAttributes( directory.c_str() );

        return (code != 0xFFFFFFFF && (FILE_ATTRIBUTE_DIRECTORY & code) != 0);
    }

    bool ForceDirectories( const std::wstring& dir )
    {
        bool    result = true;

        if ( dir.empty() )
            throw cclib::BaseException( "Cannot create directory" );

        std::wstring  new_dir = ExcludeTrailingPathDelimiter( dir );
        // last test below avoids 'xyz:\' problem.
        if ( new_dir.length() < 3 || DirectoryExists( new_dir ) || ExtractFilePath( new_dir ) == new_dir )
            return result;
        return ForceDirectories( ExtractFilePath( new_dir ) ) && CreateDir( new_dir );
    }

    std::wstring IncludeTrailingPathDelimiter( const std::wstring& str )
    {
        std::wstring    result( str );

        if ( result.length() > 0 && result[result.length() - 1] != PathDelim )
            result.append( 1, PathDelim );
        return result;
    }

    std::wstring ExcludeTrailingPathDelimiter( const std::wstring& str )
    {
        std::wstring::size_type     len = str.length();

        if ( len > 0 && str[len - 1] == PathDelim )
            return str.substr( 0, len - 1 );
        return str;
    }

}
