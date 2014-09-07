#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace dvc
{
    /************************************************************
    ********    TStringList
    ************************************************************/
    template <class CH> class TStringList
    {
    private:
        typedef std::basic_string< CH, std::char_traits<CH>, std::allocator<CH> >       std_string;
        typedef std::vector<std_string>                                                 container;
    public:
        typedef container::size_type        size_type;
        static const size_type              npos = 0xFFFFFFFF;
    private:
        container           mList;
        static const CH     NameValueSeparator = TEXT('=');

        container::iterator IndexOfName( const std_string& str );

        static bool read_string( std::string& sstr, std::istream& st )
        {
            std::vector<char>   buff;

            sstr.clear();
            while ( ! st.eof() )
            {
                std::vector<char>::size_type    size = buff.size();

                buff.resize( size + 4096 );

                char    *new_buff = &buff[size];

                st.getline( new_buff, 4096 );
                if ( st.bad() )
                    throw BaseException( DelphiMsg::SFileReadError, __LINE__, __FILE__ );
                if ( ! st.fail() || st.eof() )
                {
                    sstr = std::string( &buff.front() );
                    break;
                }
                else
                    st.clear();
            }
            return ( ! st.eof() );
        }
        static void LoadFromStream( container& container, std::istream& st )
        {
            std::string         sstr;

            while ( read_string( sstr, st ) )
                container.push_back( LocalString( sstr ) );
        }
        static void LoadFromFile( container& container, const std::string& fname )
        {
            std::ifstream   st( fname.c_str(), std::ios_base::in );

            if ( ! st.fail() )
                LoadFromStream( container, st );
        }
        static void LoadFromFile( container& container, const char *fname )
        {
            LoadFromFile( container, std::string( fname ) );
        }
    public:
        TStringList() : mList()                 {}
        ~TStringList()                          {}

        const std_string& operator[]( size_type idx ) const         { return mList.at( idx ); }
        std_string& operator[]( size_type idx )                     { return mList.at( idx ); }

        void Add( const container::value_type& str );
        void Clear();
        size_type IndexOf( const std_string& str );
        void TrimBack();
        size_type Count() const;
        void Text( const std_string& value );
        void LoadFromFile( const std_string& str )
        {
            LoadFromFile( mList, NarrowString( str ) );
        }

        std_string Names( size_type idx ) const;
        std_string Values( size_type idx );
        std_string Values( const std_string& name );
        std_string Values( const std_string::value_type *name );
    };
}
// namespace dvc
