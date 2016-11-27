#pragma once

#ifndef CCLIB_WIN_CLASSES_H
#define CCLIB_WIN_CLASSES_H

#include <string>
#include <vector>
#include "win_str.h"
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
                    return (ch == CharConstant<wchar_t>::lf || ch == CharConstant<wchar_t>::cr);
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

                while ( ch != CharConstant<wchar_t>::lf && ch != CharConstant<wchar_t>::cr )
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

        const std::wstring& operator[]( size_type idx ) const;
        std::wstring& operator[]( size_type idx );

        void Add( const std::wstring& str );
        void Clear();
        void Delete( size_type idx );
        size_type Count() const;

        size_type IndexOf( const std::wstring& str );
        void TrimBack();
        void Text( const std::wstring& value );
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
        typedef cclib::array<wchar_t, 2048>     TBuffer;
    private:
        std::wstring    FFileName;

        int ReadProfile( const wchar_t *section, const wchar_t *key, const wchar_t *def, wchar_t *out, int out_size )
        {
            return GetPrivateProfileString( section, key, def, out, out_size, FFileName.c_str() );
        }

        void FillStringList( const TBuffer& buffer, TStringList& list );

        // noncopyable
        TIniFile( const TIniFile& src );
        TIniFile& operator = ( const TIniFile& src );
    public:
        TIniFile( const std::wstring& file_name );
        ~TIniFile();

        void ReadSections( TStringList& list );
        void ReadSectionKeys( const wchar_t *section, TStringList& list );

        bool ReadBool( const wchar_t *section, const wchar_t *key, bool def );
        int ReadInteger( const wchar_t *section, const wchar_t *key, int def );
        std::wstring ReadString( const wchar_t *section, const wchar_t *key, const std::wstring& def );
    };
#pragma endregion

}
#endif
