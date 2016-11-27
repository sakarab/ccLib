#include <pre_cc.h>
#include "winClasses.h"
#include "smException.h"
#include <boost/lexical_cast.hpp>

namespace ccwin
{

#pragma region TStringList
    /************************************************************
    ********    TStringList
    ************************************************************/
    TStringList::TStringList()
        : mList()
    {}

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
        class find_func : public std::unary_function<std::wstring, bool>
        {
        private:
            std::wstring  mValue;
        public:
            find_func( const std::wstring& str ) : mValue( str ) {} // empty
            result_type operator()( const argument_type& str )
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
                (*it)[str_len] == CharConstant<wchar_t>::equal &&
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

    void TStringList::Text( const std::wstring& value )
    {
        TextIT( value.begin(), value.end() );
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
        std::wstring::size_type     pos = result.find( CharConstant<wchar_t>::equal );

        if ( pos != std::wstring::npos )
            return result.substr( 0, pos );
        return result;
    }

    std::wstring TStringList::Values( size_type idx )
    {
        std::wstring                result = operator[]( idx );
        std::wstring::size_type     pos = result.find( CharConstant<wchar_t>::equal );

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
        : FFileName( file_name )
    {
    }

    TIniFile::~TIniFile()
    {}

    void TIniFile::FillStringList( const TBuffer& buffer, TStringList& list )
    {
        const wchar_t   *ptr = &buffer.front();

        while ( *ptr != 0 )
        {
            list.Add( std::wstring( ptr ) );
            ptr += wcslen( ptr ) + 1;
        }
    }

    void TIniFile::ReadSections( TStringList& list )
    {
        list.Clear();

        TBuffer     buffer;
        int         len = ReadProfile( nullptr, nullptr, nullptr, &buffer.front(), buffer.size() );

        if ( len > 0 )
            FillStringList( buffer, list );
    }

    void TIniFile::ReadSectionKeys( const wchar_t *section, TStringList& list )
    {
        list.Clear();

        TBuffer     buffer;
        int         len = ReadProfile( section, nullptr, nullptr, &buffer.front(), buffer.size() );

        if ( len > 0 )
            FillStringList( buffer, list );
    }

    bool TIniFile::ReadBool( const wchar_t *section, const wchar_t *key, bool def )
    {
        return ReadInteger( section, key, def ? 1 : 0 ) != 0;
    }

    int TIniFile::ReadInteger( const wchar_t *section, const wchar_t *key, int def )
    {
        int             result = def;
        std::wstring    int_str = ReadString( section, key, std::wstring() );

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

    std::wstring TIniFile::ReadString( const wchar_t *section, const wchar_t *key, const std_string& def )
    {
        cclib::array<wchar_t, 2048>     buffer;
        int                             len = ReadProfile( section, key, def.c_str(), &buffer.front(), buffer.size() );

        return std::wstring( &buffer.front(), len );
    }
#pragma endregion

}
