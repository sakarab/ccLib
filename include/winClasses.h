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

#ifndef CCLIB_WIN_CLASSES_H
#define CCLIB_WIN_CLASSES_H

#include <string>
#include <vector>
#include "cpp_string.h"
#include "cc_array.hpp"
#include "ITStream.h"

namespace ccwin
{
#pragma region TStringList
    /************************************************************
    ********    TStringList
    ************************************************************/
    class TStringList
    {
    private:
        typedef std::vector<std::wstring>       container;
    public:
        typedef container::size_type        size_type;
        static const size_type              npos = 0xFFFFFFFF;
    private:
        container           mList;

        container::iterator IteratorOfName( const std::wstring& str );

        template <class IT> void TextIT( IT begin, IT end )
        {
            Clear();

            if ( begin == end )
                return;

            while ( begin != end )
            {
                IT          start = begin;
                wchar_t     ch = *begin;

                while ( ch != cclib::CharConstant<wchar_t>::lf && ch != cclib::CharConstant<wchar_t>::cr )
                {
                    ++begin;
                    if ( begin == end )
                        break;
                    ch = *begin;
                }
                mList.push_back( std::wstring( start, begin ) );
                cclib::AdvanceOverCRLF( begin, end, ch );
            }
        }
    public:
        TStringList();
        explicit TStringList( const container& list );
#if defined(CC_HAVE_MOVE_CTOR)
        explicit TStringList( container&& list );
#endif
        ~TStringList();

#if defined(CC_HAVE_COPY_DEFAULT)
        TStringList( const TStringList& other ) = default;
        TStringList( TStringList&& other ) = default;
        TStringList& operator=( const TStringList& other ) = default;
        TStringList& operator=( TStringList&& other ) = default;
#endif

        const std::wstring& operator[]( size_type idx ) const;
        std::wstring& operator[]( size_type idx );

        void Add( const std::wstring& str );
        void Clear();
        void Delete( size_type idx );
        size_type Count() const;

        size_type IndexOf( const std::wstring& str );
        void TrimBack();
        std::wstring Text() const;
        void Text( const std::wstring& value );
        void DelimitedText( const std::wstring& value, wchar_t delimiter );
        std::wstring DelimitedText( wchar_t delimiter ) const;
        int IndexOfName( const std::wstring& str );
        std::wstring Names( size_type idx ) const;
        std::wstring Values( size_type idx );
        std::wstring Values( const std::wstring& name );
        std::wstring Values( const std::wstring::value_type *name );
    };
#pragma endregion

#pragma region TIniFile
    /************************************************************
    ********    TIniFile
    ************************************************************/
    class TIniFile
    {
    private:
        typedef cclib::array<wchar_t, 8192>     TBuffer;
        typedef cclib::array<wchar_t, 32767>    TLargeBuffer;
    private:
        std::wstring    mFileName;
        bool            mDirectoryExists;

        DWORD ReadProfile( const wchar_t *section, const wchar_t *key, const wchar_t *def, wchar_t *out, int out_size );
        void WriteProfile( const wchar_t *section, const wchar_t *key, const wchar_t *value );
        void FillStringList( const TBuffer& buffer, TStringList& list );
        void FillStringList( const TBuffer& buffer, std::vector<std::wstring>& list );
        // noncopyable
        TIniFile( const TIniFile& src ) CC_EQ_DELETE;
        TIniFile& operator = ( const TIniFile& src ) CC_EQ_DELETE;
    public:
        explicit TIniFile( const std::wstring& file_name );
        ~TIniFile();

        const std::wstring& FileName() const                { return mFileName; }

        void ReadSections( TStringList& list );

        std::vector<std::wstring> ReadSection( const wchar_t *section );
        std::vector<std::wstring> ReadSectionKeys( const wchar_t *section );
        void ReadSectionKeys( const wchar_t *section, TStringList& list );
        void ReadSectionKeys( const wchar_t *section, std::vector<std::wstring>& list );

        void EraseSection( const wchar_t *section );
        void EraseKey( const wchar_t *section, const wchar_t *key );

        bool ReadBool( const wchar_t *section, const wchar_t *key, bool def );
        int ReadInteger( const wchar_t *section, const wchar_t *key, int def );
        std::wstring ReadString( const wchar_t *section, const wchar_t *key, const wchar_t *def );
        bool Read( const wchar_t *section, const wchar_t *key, bool def )                       { return ReadBool( section, key, def ); }
        int Read( const wchar_t *section, const wchar_t *key, int def )                         { return ReadInteger( section, key, def ); }
        std::wstring Read( const wchar_t *section, const wchar_t *key, const wchar_t *def )     { return ReadString( section, key, def ); }

        void WriteBool( const wchar_t *section, const wchar_t *key, bool value );
        void WriteInteger( const wchar_t *section, const wchar_t *key, int value );
        void WriteString( const wchar_t *section, const wchar_t *key, const wchar_t *value );
        void Write( const wchar_t *section, const wchar_t *key, bool value )                    { WriteBool( section, key, value ); }
        void Write( const wchar_t *section, const wchar_t *key, int value )                     { WriteInteger( section, key, value ); }
        void Write( const wchar_t *section, const wchar_t *key, const wchar_t *value )          { WriteString( section, key, value ); }
    };
#pragma endregion

#pragma region TRegistry
    /************************************************************
    ********    TRegistry
    ************************************************************/
    class TRegistry
    {
    public:
        enum TRegDataType { rdUnknown, rdString, rdExpandString, rdInteger, rdBinary };
        struct TRegDataInfo
        {
            DWORD           DataSize;
            TRegDataType    RegData;
        };
        struct TRegKeyInfo
        {
            DWORD       SubKeys;
            DWORD       MaxSubKeyLen;
            DWORD       Values;
            DWORD       MaxValueNameLen;
            DWORD       MaxValueLen;
            FILETIME    LastWriteTime;
        };
    private:
        HKEY            FRootKey;
        HKEY            FCurrentKey;
        std_string      FCurrentPath;
        unsigned int    FAccess;
        bool            FLazyWrite;
        bool            FCloseRootKey;

        HKEY GetBaseKey( bool relative );
        void ChangeKey( HKEY value, const std::wstring& path );
        int GetDataSize( const std::wstring& value_name );
        bool GetDataInfo( const std::wstring& value_name, TRegDataInfo& value );
        int GetData( const std::wstring& name, BYTE *buffer, DWORD buf_size, TRegDataType& reg_data );
        void PutData( const std::wstring& name, const BYTE *buffer, DWORD buf_size, TRegDataType reg_data );
        // noncopyable
        TRegistry( const TRegistry& src );
        TRegistry& operator = ( const TRegistry& src );
    public:
        TRegistry();
        TRegistry( DWORD access_rights );
        ~TRegistry();
        void RootKey( HKEY key );
        bool OpenKey( const std::wstring& key, bool can_create );
        bool OpenKey( const wchar_t *key, bool can_create );
        void CloseKey();
        bool ValueExists( const std::wstring& key );
        bool ValueExists( const wchar_t *key );
        bool ReadBool( const std::wstring& key );
        int  ReadInteger( const std::wstring& key );
        std::wstring ReadString( const std::wstring& key );
        int ReadBinaryData( const std::wstring& key, void *buffer, int buf_size );
        void WriteBinaryData( const std::wstring& key, void *buffer, int buf_size );
        void WriteString( const std::wstring& name, const std::wstring& value );
        //std::string RegGetValueA( const char *key );
        bool GetKeyInfo( TRegKeyInfo& value );

        HKEY GetCurrentKey() const { return FCurrentKey; }
    };
#pragma endregion

#pragma region TFileStreamEx
    //===========================================================
    //======    TFileStreamEx
    //===========================================================
    class TFileStreamEx : public cclib::ITStream
    {
    public:
        // open modes
        static const WORD rfmCreateNew      = CREATE_NEW;
        static const WORD rfmCreateAlways   = CREATE_ALWAYS;
        static const WORD rfmOpenExisting   = OPEN_EXISTING;
        static const WORD rfmOpenAlways     = OPEN_ALWAYS;

        static const WORD fmOpenRead        = 0x00;
        static const WORD fmOpenWrite       = 0x01;
        static const WORD fmOpenReadWrite   = 0x02;

        static const WORD fmShareCompat     = 0x00;
        static const WORD fmShareExclusive  = 0x10;
        static const WORD fmShareDenyWrite  = 0x20;
        static const WORD fmShareDenyRead   = 0x30;
        static const WORD fmShareDenyNone   = 0x40;
    private:
        HANDLE          mHandle;
        std_string      mFileName;
        DWORD           mLastError;
        // ITStream
        virtual void Raise_LastError() CC_OVERRIDE;
        virtual size_t ReadInternal( void *mem, size_t count ) CC_OVERRIDE;
        virtual size_t WriteInternal( const void *mem, size_t count ) CC_OVERRIDE;
        virtual size_type get_position() const CC_OVERRIDE;
        virtual void set_position( size_type value ) CC_OVERRIDE;
        virtual size_type get_size() const CC_OVERRIDE;
        virtual void set_size( size_type value ) CC_OVERRIDE;
        // ITStream end

        void UnableTo( const char *message, DWORD last_os_error );
        bool IsOpen()                                               { return ( mHandle != INVALID_HANDLE_VALUE ); }
        void Open( const TCHAR *fname, WORD create_mode, WORD access_share_mode );
        void Close();

        // noncopyable
        TFileStreamEx( const TFileStreamEx& src ) CC_EQ_DELETE;
        TFileStreamEx& operator = ( const TFileStreamEx& src ) CC_EQ_DELETE;
    public:
        TFileStreamEx( const TCHAR *FileName, WORD create_mode, WORD access_share_mode );
        TFileStreamEx( const std_string& FileName, WORD create_mode, WORD access_share_mode );
        ~TFileStreamEx();

        HANDLE GetHandle() const            { return mHandle; }
    };
#pragma endregion

}
#endif
