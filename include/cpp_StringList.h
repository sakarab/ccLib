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

#ifndef CPP_STRING_LIST_H
#define CPP_STRING_LIST_H

#include <string>
#include <vector>

namespace cclib
{
#pragma region DelimitedText
    // string DelimitedText( vector<string> list, char delimiter )
    template <class CH>
    std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>
        DelimitedText( const std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>& list, CH delimiter )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        std::vector<CH>         buffer;

        for ( const string_type& sstr : list )
        {
            buffer.insert( buffer.end(), sstr.begin(), sstr.end() );
            buffer.push_back( delimiter );
        }
        if ( buffer.empty() )
            return str_type();
        else
            buffer.resize( buffer.size() - 1 );
        return str_type( &buffer.front(), buffer.size() );
    }

    // vector<string> DelimitedText( string text, char delimiter )
    // void DelimitedText( vector<string>, string text, char delimiter )
    template <class CH>
    void DelimitedText( std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>& list,
                        const std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>& sstr,
                        CH delimiter )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        std::size_t     start = 0;
        std::size_t     end_pos = value.find_first_of( delimiter, start );

        while ( end_pos != str_type::npos )
        {
            list.push_back( value.substr( start, end_pos - start ) );
            start = end_pos + 1;
            end_pos = value.find_first_of( delimiter, start );
        }
        if ( start < value.length() )
            list.push_back( value.substr( start ) );
    }

    template <class CH>
    std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>
        DelimitedText( const std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>& sstr, CH delimiter )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        list_type       result;

        DelimitedText( result, sstr, delimiter );
        return result;
    }
#pragma endregion

#pragma region Text
    // string from vector<string>
    template <class CH>
    std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>
        Text( const std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>& list )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        std::vector<CH>     buffer;

        for ( const string_type& sstr : list )
        {
            buffer.insert( buffer.end(), sstr.begin(), sstr.end() );
            buffer.push_back( CharConstant<CH>::cr );
            buffer.push_back( CharConstant<CH>::lf );
        }
        if ( buffer.empty() )
            return string_type();
        return string_type( &buffer.front(), buffer.size() );
    }

    // vector from string iterators
    template <class IT, class CH>
    void Text( std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>& list,
               const IT& begin, const IT& end )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        if ( begin == end )
            return;
        while ( begin != end )
        {
            IT      start = begin;
            CH      ch = *begin;

            while ( ch != CharConstant<CH>::cr && ch != CharConstant<CH>::lf )
            {
                ++begin;
                if ( begin == end )
                    break;
                ch = *begin;
            }
            list.push_back( string_type( start, begin ) );
            AdvanceOverCRLF( begin, end, ch );
        }
    }

    template <class IT, class CH>
    std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>
        Text( const IT& begin, const IT& end )
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;

        list_type   result;

        TextIT( result, begin, end );
        return result;
    }

    // vector from string
    template <class IT, class CH>
    void Text( std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>& list,
               const std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>& sstr )
    {
        Text( list, sstr.begin(), sstr.end() );
    }

    template <class IT, class CH>
    std::vector<std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>>
        Text( const std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>& sstr )
    {
        return Text( sstr.begin(), sstr.end() );
    }
#pragma endregion
}

#endif