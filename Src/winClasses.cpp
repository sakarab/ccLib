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

#include <pre_cc.h>
#include "winClasses.h"
#include "win_str.h"
#include "smException.h"
#include "cc_memory.hpp"
#include <boost/lexical_cast.hpp>
#include "winUtils.h"
#include "winOSUtils.h"

namespace
{

    bool IsRelative( const std_string& str )
    {
        return !(!str.empty() && str[0] == L'\\');
    }

    ccwin::TRegistry::TRegDataType DataTypeToRegData( int value )
    {
        if ( value == REG_SZ )
            return ccwin::TRegistry::rdString;
        if ( value == REG_EXPAND_SZ )
            return ccwin::TRegistry::rdExpandString;
        if ( value == REG_DWORD )
            return ccwin::TRegistry::rdInteger;
        if ( value == REG_BINARY )
            return ccwin::TRegistry::rdBinary;
        return ccwin::TRegistry::rdUnknown;
    }

    int RegDataToDataType( ccwin::TRegistry::TRegDataType data_type )
    {
        if ( data_type == ccwin::TRegistry::rdString )
            return REG_SZ;
        if ( data_type == ccwin::TRegistry::rdExpandString )
            return REG_EXPAND_SZ;
        if ( data_type == ccwin::TRegistry::rdInteger )
            return REG_DWORD;
        if ( data_type == ccwin::TRegistry::rdBinary )
            return REG_BINARY;
        return REG_NONE;
    }

    void ReadError( const std::wstring& name )
    {
#if defined (CC_HAVE_FMT_FORMAT)
        throw cclib::BaseException( fmt::format( "InvalidRegistryType {1}", ccwin::NarrowStringStrict( name ) ) );
#else
        throw cclib::BaseException( boost::str( boost::format( "InvalidRegistryType %1%" ) % ccwin::NarrowStringStrict( name ) ) );
#endif
    }

    /************************************************************
    ********    TLargeInteger
    ************************************************************/
    class TLargeInteger
    {
    private:
        LARGE_INTEGER   mVal;
    public:
        explicit TLargeInteger( int val )
        {
            mVal.u.LowPart = val;
            mVal.u.HighPart = 0;
        }
        explicit TLargeInteger( long long val )
        {
            mVal.QuadPart = val;
        }

        LARGE_INTEGER& val()            { return mVal; }
        operator LARGE_INTEGER()        { return mVal; }
        operator long long()            { return mVal.QuadPart; }
    };

}
// namespace

namespace ccwin
{

#pragma region TStringList
    /************************************************************
    ********    TStringList
    ************************************************************/
    TStringList::TStringList()
        : mList()
    {
    }

    TStringList::TStringList( const container& list )
        : mList( list )
    {
    }

#if defined(CC_HAVE_MOVE_CTOR)
    TStringList::TStringList( container&& list )
        : mList( std::move(list) )
    {
    }
#endif

    TStringList::~TStringList()
    {}

    const std::wstring& TStringList::operator[]( size_type idx ) const
    {
        if ( idx < mList.size() )
            return mList[idx];
        throw cclib::BaseException( "List Out Of Index" );
    }

    std::wstring& TStringList::operator[]( size_type idx )
    {
        if ( idx < mList.size() )
            return mList[idx];
        throw cclib::BaseException( "List Out Of Index" );
    }

    TStringList::size_type TStringList::IndexOf( const std::wstring& str )
    {
        class find_func
        {
        private:
            std::wstring  mValue;
        public:
            find_func( const std::wstring& str ) : mValue( str ) {} // empty
            bool operator()( const std::wstring& str )
            {
                return CompareText( mValue, str ) == 0;
            }
        };

        container::iterator     it = std::find_if( mList.begin(), mList.end(), find_func( str ) );

        if ( it != mList.end() )
            return std::distance( mList.begin(), it );
        return npos;
    }

    TStringList::container::iterator TStringList::IteratorOfName( const std::wstring& str )
    {
        std::wstring::size_type     str_len = str.length();

        for ( container::iterator it = mList.begin(), eend = mList.end() ; it != eend ; ++it )
        {
            if ( it->length() > str_len &&
                (*it)[str_len] == cclib::CharConstant<wchar_t>::equal &&
                 wcsncmp( it->c_str(), str.c_str(), str_len ) == 0 )
            {
                return it;
            }
        }
        return mList.end();
    }

    int TStringList::IndexOfName( const std::wstring& str )
    {
        container::iterator     it = IteratorOfName( str );

        if ( it != mList.end() )
            return std::distance( mList.begin(), it );
        return -1;
    }

    void TStringList::Clear()
    {
        container   tmp;

        mList.swap( tmp );
    }

    void TStringList::Delete( size_type idx )
    {
        if ( idx >= mList.size() )
            throw cclib::BaseException( "List Out Of Index" );

        container::iterator     it = mList.begin();

        std::advance( it, idx );
        mList.erase( it );
    }

    void TStringList::TrimBack()
    {
        container::reverse_iterator     rit = mList.rbegin(),
            reend = mList.rend();

        while ( rit != reend && Trim( *rit ).empty() )
            ++rit;
        mList.erase( rit.base() );
    }

    std::wstring TStringList::Text() const
    {
        std::vector<wchar_t>        buffer;

        for ( size_type n = 0, eend = Count() ; n < eend ; ++n )
        {
            const std::wstring&     sstr = mList[n];

            buffer.insert( buffer.end(), sstr.begin(), sstr.end() );
            buffer.push_back( cclib::CharConstant<wchar_t>::cr );
            buffer.push_back( cclib::CharConstant<wchar_t>::lf );
        }
        if ( buffer.empty() )
            return std::wstring();
        return std::wstring( &buffer.front(), buffer.size() );
    }

    void TStringList::Text( const std::wstring& value )
    {
        TextIT( value.begin(), value.end() );
    }

    void TStringList::DelimitedText( const std::wstring& value, wchar_t delimiter )
    {
        container       slist;
        std::size_t     start = 0;
        std::size_t     end_pos = value.find_first_of( delimiter, start );

        while ( end_pos != std::wstring::npos )
        {
            slist.push_back( value.substr( start, end_pos - start ) );
            start = end_pos + 1;
            end_pos = value.find_first_of( delimiter, start );
        }
        if ( start < value.length() )
            slist.push_back( value.substr( start ) );
        mList = slist;
    }

    std::wstring TStringList::DelimitedText( wchar_t delimiter ) const
    {
        std::vector<wchar_t>        buffer;

        for ( size_type n = 0, eend = Count() ; n < eend ; ++n )
        {
            const std::wstring&     sstr = mList[n];

            buffer.insert( buffer.end(), sstr.begin(), sstr.end() );
            buffer.push_back( delimiter );
        }
        if ( buffer.empty() )
            return std::wstring();
        else
            buffer.resize( buffer.size() - 1 );
        return std::wstring( &buffer.front(), buffer.size() );
    }

    TStringList::size_type TStringList::Count() const
    {
        return mList.size();
    }

    void TStringList::Add( const std::wstring& str )
    {
        mList.push_back( str );
    }

    std::wstring TStringList::Names( size_type idx ) const
    {
        std::wstring                result = operator[]( idx );
        std::wstring::size_type     pos = result.find( cclib::CharConstant<wchar_t>::equal );

        if ( pos != std::wstring::npos )
            return result.substr( 0, pos );
        return result;
    }

    std::wstring TStringList::Values( size_type idx )
    {
        std::wstring                result = operator[]( idx );
        std::wstring::size_type     pos = result.find( cclib::CharConstant<wchar_t>::equal );

        if ( pos != std_string::npos )
            return result.substr( pos + 1 );
        return std::wstring();
    }

    std::wstring TStringList::Values( const std::wstring& name )
    {
        container::iterator     it = IteratorOfName( name );

        if ( it != mList.end() )
            return it->substr( name.length() + 1, it->max_size() );
        return std::wstring();
    }

    std::wstring TStringList::Values( const std::wstring::value_type *name )
    {
        return Values( container::value_type( name ) );
    }
#pragma endregion

#pragma region TIniFile
    /************************************************************
    ********    TIniFile
    ************************************************************/
    TIniFile::TIniFile( const std_string& file_name )
        : mFileName( file_name ), mDirectoryExists( DirectoryExists( ExtractFilePath( file_name ) ) )
    {
        std_string  path = ExtractFilePath( file_name );

        if ( path.empty() )
            mDirectoryExists = true;
        else
            mDirectoryExists = DirectoryExists( path );
    }

    TIniFile::~TIniFile()
    {}

    DWORD TIniFile::ReadProfile( const wchar_t * section, const wchar_t * key, const wchar_t * def, wchar_t * out, int out_size )
    {
        return GetPrivateProfileString( section, key, def, out, out_size, mFileName.c_str() );
    }

    void TIniFile::WriteProfile( const wchar_t * section, const wchar_t * key, const wchar_t * value )
    {
        if ( !mDirectoryExists )
        {
            ForceDirectories( ExtractFilePath( mFileName ) );
            mDirectoryExists = true;
        }
        if ( ! WritePrivateProfileString( section, key, value, mFileName.c_str() ) )
            RaiseLastOSError();
    }

    void TIniFile::FillStringList( const TBuffer& buffer, TStringList& list )
    {
        const wchar_t   *ptr = &buffer.front();

        while ( *ptr != 0 )
        {
            list.Add( std::wstring( ptr ) );
            ptr += wcslen( ptr ) + 1;
        }
    }

    void TIniFile::FillStringList( const TBuffer& buffer, std::vector<std::wstring>& list )
    {
        const wchar_t           *ptr = &buffer.front();
        TBuffer::size_type      idx = 0;

        while ( *ptr != 0 && idx < buffer.size() )
        {
            list.push_back( std::wstring( ptr ) );

            TBuffer::size_type      diff = list.back().length() + 1;

            ptr += diff;
            idx += diff;
        }
    }

    void TIniFile::ReadSections( TStringList& list )
    {
        list.Clear();

        TBuffer     buffer;
        DWORD       len = ReadProfile( nullptr, nullptr, nullptr, &buffer.front(), buffer.size() );

        if ( len > 0 )
            FillStringList( buffer, list );
    }

    std::vector<std::wstring> TIniFile::ReadSection( const wchar_t * section )
    {
#if defined (CC_HAVE_SHARED_PTR)
        std::unique_ptr<TLargeBuffer>   buffer = std::make_unique<TLargeBuffer>();
#else
        std::auto_ptr<TLargeBuffer>		buffer = std::auto_ptr<TLargeBuffer>( new TLargeBuffer() );
#endif
        TLargeBuffer::value_type        *bptr = &(buffer->front());
        DWORD                           bsize = GetPrivateProfileSection( section, bptr, buffer->size(), mFileName.c_str() );
        std::vector<std::wstring>       result;
        DWORD                           total_length = 0;

        while ( *bptr != 0 && total_length < bsize )
        {
            result.push_back( bptr );

            DWORD   plus = result.back().length() + 1;

            bptr += plus;
            total_length += plus;
        }
        return result;
    }

    std::vector<std::wstring> TIniFile::ReadSectionKeys( const wchar_t * section )
    {
        std::vector<std::wstring>   result;

        ReadSectionKeys( section, result );
        return result;
    }

    void TIniFile::ReadSectionKeys( const wchar_t *section, TStringList& list )
    {
        list.Clear();

        TBuffer     buffer;
        DWORD       len = ReadProfile( section, nullptr, nullptr, &buffer.front(), buffer.size() );

        if ( len > 0 )
            FillStringList( buffer, list );
    }

    void TIniFile::ReadSectionKeys( const wchar_t * section, std::vector<std::wstring>& list )
    {
        list.clear();

        TBuffer     buffer;
        DWORD       len = ReadProfile( section, nullptr, nullptr, &buffer.front(), buffer.size() );

        if ( len > 0 )
            FillStringList( buffer, list );
    }

    void TIniFile::EraseSection( const wchar_t * section )
    {
        WriteProfile( section, 0, 0 );
    }

    void TIniFile::EraseKey( const wchar_t * section, const wchar_t * key )
    {
        WriteProfile( section, key, 0 );
    }

    bool TIniFile::ReadBool( const wchar_t *section, const wchar_t *key, bool def )
    {
        return ReadInteger( section, key, def ? 1 : 0 ) != 0;
    }

    int TIniFile::ReadInteger( const wchar_t *section, const wchar_t *key, int def )
    {
        int             result = def;
        std::wstring    int_str = ReadString( section, key, L"" );

        if ( !int_str.empty() )
        {
            try
            {
                result = boost::lexical_cast<int>(int_str);
            }
            catch ( const boost::bad_lexical_cast& )
            {
            }
        }
        return result;
    }

    std::wstring TIniFile::ReadString( const wchar_t *section, const wchar_t *key, const wchar_t *def )
    {
        TBuffer     buffer;
        DWORD       len = ReadProfile( section, key, def, &buffer.front(), buffer.size() );

        return std::wstring( &buffer.front(), len );
    }

    void TIniFile::WriteBool( const wchar_t * section, const wchar_t * key, bool value )
    {
        WriteInteger( section, key, value ? 1 : 0 );
    }

    void TIniFile::WriteInteger( const wchar_t * section, const wchar_t * key, int value )
    {
#if defined (CC_HAVE_FMT_FORMAT)
        WriteString( section, key, fmt::format( L"{1}", value ).c_str() );
#else
        WriteString( section, key, boost::str( boost::wformat( L"%1%" ) % value ).c_str() );
#endif
    }

    void TIniFile::WriteString( const wchar_t *section, const wchar_t *key, const wchar_t *value )
    {
        WriteProfile( section, key, value );
    }
#pragma endregion

#pragma region TRegistry
    /************************************************************
    ********    TRegistry
    ************************************************************/
    TRegistry::TRegistry()
        : FRootKey( HKEY_CURRENT_USER ),
        FCurrentKey(),
        FCurrentPath(),
        FAccess( KEY_ALL_ACCESS ),
        FLazyWrite( true ),
        FCloseRootKey( false )
    {
    }

    TRegistry::TRegistry( DWORD access_rights )
        : FRootKey( HKEY_CURRENT_USER ),
        FCurrentKey(),
        FCurrentPath(),
        FAccess( access_rights ),
        FLazyWrite( true ),
        FCloseRootKey( false )
    {
    }

    TRegistry::~TRegistry()
    {
        CloseKey();
    }

    void TRegistry::RootKey( HKEY key )
    {
        if ( FRootKey != key )
        {
            if ( FCloseRootKey )
            {
                RegCloseKey( FRootKey );
                FCloseRootKey = false;
            }
            FRootKey = key;
            CloseKey();
        }
    }

    void TRegistry::CloseKey()
    {
        if ( FCurrentKey != 0 )
        {
            if ( !FLazyWrite )
                RegFlushKey( FCurrentKey );
            RegCloseKey( FCurrentKey );
            FCurrentKey = 0;
            FCurrentPath.clear();
        }
    }

    HKEY TRegistry::GetBaseKey( bool relative )
    {
        if ( FCurrentKey == 0 || !relative )
            return FRootKey;
        return FCurrentKey;
    }

    void TRegistry::ChangeKey( HKEY value, const std::wstring& path )
    {
        CloseKey();
        FCurrentKey = value;
        FCurrentPath = path;
    }

    bool TRegistry::OpenKey( const wchar_t *key, bool can_create )
    {
        return OpenKey( std::wstring( key ), can_create );
    }

    bool TRegistry::OpenKey( const std::wstring& key, bool can_create )
    {
        std::wstring    S( key );
        bool            Relative( IsRelative( S ) );

        if ( !Relative )
            S.erase( 0, 1 );

        HKEY            TempKey = 0;
        bool            result;

        if ( !can_create || S.empty() )
        {
            result = RegOpenKeyEx( GetBaseKey( Relative ), S.c_str(), 0, FAccess, &TempKey ) == ERROR_SUCCESS;
        }
        else
        {
            DWORD   Disposition;

            result = RegCreateKeyEx( GetBaseKey( Relative ), S.c_str(), 0, 0, REG_OPTION_NON_VOLATILE,
                                     FAccess, 0, &TempKey, &Disposition ) == ERROR_SUCCESS;
        }

        if ( result )
        {
            if ( FCurrentKey != 0 && Relative )
                S = FCurrentPath + std::wstring( TEXT( "\\" ) ) + S;
            ChangeKey( TempKey, S );
        }
        return result;
    }

    bool TRegistry::GetDataInfo( const std::wstring& value_name, TRegDataInfo& value )
    {
        std::memset( &value, sizeof( value ), 0 );

        DWORD   data_type;
        bool    result = RegQueryValueEx( FCurrentKey, value_name.c_str(), 0, &data_type, 0, &value.DataSize ) == ERROR_SUCCESS;

        if ( result )
            value.RegData = DataTypeToRegData( data_type );
        return result;
    }

    bool TRegistry::GetKeyInfo( TRegKeyInfo& value )
    {
        std::memset( &value, sizeof( value ), 0 );

        LSTATUS     result = RegQueryInfoKey( FCurrentKey, 0, 0, 0, &value.SubKeys,
                                              &value.MaxSubKeyLen, 0, &value.Values, &value.MaxValueNameLen,
                                              &value.MaxValueLen, 0, &value.LastWriteTime );

        value.MaxSubKeyLen *= 2;
        value.MaxValueNameLen *= 2;
        return result == ERROR_SUCCESS;
    }

    int TRegistry::GetDataSize( const std::wstring& value_name )
    {
        TRegDataInfo    value;

        if ( GetDataInfo( value_name, value ) )
            return value.DataSize;
        return -1;
    }

    bool TRegistry::ValueExists( const wchar_t *key )
    {
        return ValueExists( std::wstring( key ) );
    }

    bool TRegistry::ValueExists( const std::wstring& key )
    {
        TRegDataInfo    info;

        return GetDataInfo( key, info );
    }

    int TRegistry::GetData( const std::wstring& name, BYTE *buffer, DWORD buf_size, TRegDataType& reg_data )
    {
        DWORD   data_type = REG_NONE;

        if ( RegQueryValueEx( FCurrentKey, name.c_str(), 0, &data_type, buffer, &buf_size ) != ERROR_SUCCESS )
            throw cclib::BaseException( "TRegistry::GetData() failed" );
        reg_data = DataTypeToRegData( data_type );
        return buf_size;
    }

    void TRegistry::PutData( const std::wstring& name, const BYTE *buffer, DWORD buf_size, TRegDataType reg_data )
    {
        DWORD   data_type = RegDataToDataType( reg_data );

        if ( RegSetValueEx( FCurrentKey, name.c_str(), 0, data_type, buffer, buf_size ) != ERROR_SUCCESS )
            throw cclib::BaseException( "TRegistry::PutData() failed" );
    }

    int TRegistry::ReadInteger( const std::wstring& key )
    {
        TRegDataType    reg_data;
        int             result;

        GetData( key, reinterpret_cast<BYTE *>(&result), sizeof( int ), reg_data );
        if ( reg_data != rdInteger )
            ReadError( key );
        return result;
    }

    bool TRegistry::ReadBool( const std::wstring& key )
    {
        return ReadInteger( key ) != 0;
    }

    std::wstring TRegistry::ReadString( const std::wstring& key )
    {
        int     len = GetDataSize( key );

        if ( len > 0 )
        {
            std::vector<BYTE>   buffer( len );
            TRegDataType        reg_data;

            GetData( key, &buffer.front(), len, reg_data );
            if ( reg_data == rdString || reg_data == rdExpandString )
            {
                buffer.push_back( 0 );
                buffer.push_back( 0 );
                return std_string( reinterpret_cast<TCHAR *>(&buffer.front()) );
            }
            ReadError( key );
        }
        return std_string();
    }

    int TRegistry::ReadBinaryData( const std::wstring& key, void *buffer, int buf_size )
    {
        TRegDataInfo    info;
        int             result = 0;

        if ( GetDataInfo( key, info ) )
        {
            result = info.DataSize;

            TRegDataType    reg_data = info.RegData;

            if ( (reg_data == rdBinary || reg_data == rdUnknown) && result <= buf_size )
                GetData( key, reinterpret_cast<BYTE *>(buffer), result, reg_data );
            else
                ReadError( key );
        }
        return result;
    }

    void TRegistry::WriteBinaryData( const std::wstring& key, void *buffer, int buf_size )
    {
        PutData( key, const_cast<const BYTE *>(reinterpret_cast<BYTE *>(buffer)), cclib::size_cast<DWORD>(buf_size), rdBinary );
    }

    void TRegistry::WriteString( const std_string& name, const std_string& value )
    {
        PutData( name, reinterpret_cast<const BYTE *>(value.c_str()), cclib::size_cast<DWORD>((value.length() + 1) * sizeof( TCHAR )), rdString );
    }

    //std::string TRegistry::RegGetValueA( const char *key )
    //{
    //    DWORD               buf_size = 8192;
    //    std::vector<char>   buffer( buf_size );
    //    DWORD               data_type = REG_NONE;

    //    buf_size -= 2;
    //    if ( RegQueryValueExA( FCurrentKey, key, 0, &data_type, reinterpret_cast<BYTE *>(&buffer.front()), &buf_size ) != ERROR_SUCCESS )
    //        throw BaseException( DelphiMsg::SRegGetDataFailed, __LINE__, __FILE__ );

    //    std::string     result;

    //    if ( buf_size == 0 )
    //        return result;
    //    switch ( data_type )
    //    {
    //        case REG_DWORD:
    //        case REG_BINARY:
    //            result = BinToHexA( &buffer.front(), buf_size );
    //            break;
    //        case REG_SZ:
    //        case REG_EXPAND_SZ:
    //            result.assign( &buffer.front() );
    //            break;
    //        case REG_MULTI_SZ:
    //        {
    //            char    *p = &buffer.front();
    //            char    *eend = p + buf_size;

    //            while ( *p != 0 && p < eend )
    //            {
    //                result.append( p );
    //                p += std::strlen( p ) + 1;
    //            }
    //        }
    //    }
    //    return result;
    //}
#pragma endregion

#pragma region TFileStreamEx
    //===========================================================
    //======    TFileStreamEx
    //===========================================================
    TFileStreamEx::TFileStreamEx( const TCHAR *FileName, WORD create_mode, WORD access_share_mode )
        : ITStream(), mHandle(INVALID_HANDLE_VALUE), mFileName(FileName)
    {
        Open( FileName, create_mode, access_share_mode );
    }

    TFileStreamEx::TFileStreamEx( const std_string& FileName, WORD create_mode, WORD access_share_mode )
        : ITStream(), mHandle(INVALID_HANDLE_VALUE), mFileName(FileName)
    {
        Open( FileName.c_str(), create_mode, access_share_mode );
    }

    TFileStreamEx::~TFileStreamEx()
    {
        Close();
    }

    void TFileStreamEx::UnableTo( const char *message, DWORD last_os_error )
    {
#if defined (CC_HAVE_FMT_FORMAT)
        throw std::runtime_error( fmt::format( "Unable to {3} file : {1}\n{2}", NarrowString( mFileName ), SysErrorMessage( last_os_error ), message ) );
#else
        throw std::runtime_error( boost::str( boost::format( "Unable to %3% file : %1%\n%2%" )
                                              % NarrowString( mFileName ) % SysErrorMessage( last_os_error ) % message ) );
#endif
    }

    void TFileStreamEx::Open( const TCHAR *fname, WORD create_mode, WORD access_share_mode )
    {
        static const ULONG AccessMode[3] = { GENERIC_READ, GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE };
        static const LONG  ShareMode[5]  = { 0, 0, FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE };

        if (((create_mode & 3) < rfmOpenAlways) && ((access_share_mode & 0xF0) <= fmShareDenyNone) )
        {
            mHandle = CreateFile( fname,
                                  AccessMode[access_share_mode & 3],
                                  ShareMode[(access_share_mode & 0xF0) >> 4],
                                  NULL,
                                  create_mode,
                                  FILE_ATTRIBUTE_NORMAL, 0 );

            DWORD   last_error = GetLastError();

            if ( mHandle == INVALID_HANDLE_VALUE )
            {
                if ( (create_mode == rfmCreateNew) || (create_mode == rfmCreateAlways) )
                    UnableTo( "create", last_error );
                else // rfmOpenExisting, rfmOpenAlways
                    UnableTo( "open", last_error );
            }
        }
    }

    void TFileStreamEx::Close()
    {
        if ( IsOpen() )
        {
            CloseHandle( mHandle );
            mHandle = INVALID_HANDLE_VALUE;
        }
    }

    //virtual
    void TFileStreamEx::Raise_LastError()
    {
#if defined (CC_HAVE_FMT_FORMAT)
        throw std::runtime_error( fmt::format( "Error: 0x{X}, {s}", mLastError, SysErrorMessage( mLastError ) ) );
#else
        throw std::runtime_error( boost::str( boost::format( "Error: 0x%1%, %2%" )
                                              % boost::io::group( std::uppercase, std::hex, mLastError )
                                              % SysErrorMessage( mLastError ) ) );
#endif
    }

    //virtual
    size_t TFileStreamEx::ReadInternal( void *buffer, size_t count )
    {
        DWORD   result = 0;

        if ( ReadFile( mHandle, buffer, count, &result, NULL ) == 0 )
        {
            mLastError = GetLastError();
            result = 0;
        }
        return result;
    }

    //virtual
    size_t TFileStreamEx::WriteInternal( const void *buffer, size_t count )
    {
        DWORD    result = 0;

        if ( WriteFile( mHandle, buffer, count, &result, NULL ) == 0 )
        {
            mLastError = GetLastError();
            result = 0;
        }
        return result;
    }

    //virtual
    cclib::ITStream::size_type TFileStreamEx::get_position() const
    {
        TLargeInteger   pos( 0 );

        if ( SetFilePointerEx( mHandle, TLargeInteger( 0 ), &pos.val(), FILE_CURRENT ) == 0 )
            RaiseLastOSError();
        return pos;
    }

    //virtual
    void TFileStreamEx::set_position( size_type value )
    {
        TLargeInteger   pos( value );

        if ( SetFilePointerEx( mHandle, pos, NULL, FILE_BEGIN ) == 0 )
            RaiseLastOSError();
    }

    //virtual
    cclib::ITStream::size_type TFileStreamEx::get_size() const
    {
        TLargeInteger   pos( 0 );

        if ( GetFileSizeEx( mHandle, &pos.val() ) == 0 )
            RaiseLastOSError();
        return pos;
    }

    //virtual
    void TFileStreamEx::set_size( size_type value )
    {
        TLargeInteger   pos( value );

        if ( SetFilePointerEx( mHandle, pos, NULL, FILE_BEGIN ) == 0 )
            RaiseLastOSError();
    }

#pragma endregion
}
