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

        // ch is CR or LF
        template <class IT> bool AdvanceOverCRLF( IT& it, IT end, wchar_t& ch )
        {
            if ( it != end )
            {
                ++it;
                if ( it != end )
                {
                    ch = *it;
                    return (ch == cclib::CharConstant<wchar_t>::lf || ch == cclib::CharConstant<wchar_t>::cr);
                }
            }
            return false;
        }

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

                // Do it at most twice
                if ( AdvanceOverCRLF( begin, end, ch ) )
                    AdvanceOverCRLF( begin, end, ch );
            }
        }
    public:
        TStringList();
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
    private:
        std::wstring    mFileName;
        bool            mDirectoryExists;

        int ReadProfile( const wchar_t *section, const wchar_t *key, const wchar_t *def, wchar_t *out, int out_size );
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
        void ReadSectionKeys( const wchar_t *section, TStringList& list );
        void ReadSectionKeys( const wchar_t *section, std::vector<std::wstring>& list );
        void EraseSection( const wchar_t *section );
        void EraseKey( const wchar_t *section, const wchar_t *key );

        bool ReadBool( const wchar_t *section, const wchar_t *key, bool def );
        int ReadInteger( const wchar_t *section, const wchar_t *key, int def );
        std::wstring ReadString( const wchar_t *section, const wchar_t *key, const wchar_t *def );

        void WriteBool( const wchar_t *section, const wchar_t *key, bool value );
        void WriteInteger( const wchar_t *section, const wchar_t *key, int value );
        void WriteString( const wchar_t *section, const wchar_t *key, const wchar_t *value );
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

}
#endif
