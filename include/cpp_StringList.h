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
#include <cpp_string.h>

namespace cclib
{
    template <class CH> struct type_helper
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>     string_type;
        typedef std::vector<string_type>                                            list_type;
    };

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
            return string_type();
        buffer.resize( buffer.size() - 1 );
        if ( buffer.empty() )
            return string_type();
        return string_type( &buffer.front(), buffer.size() );
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
        std::size_t     end_pos = sstr.find_first_of( delimiter, start );

        while ( end_pos != string_type::npos )
        {
            list.push_back( sstr.substr( start, end_pos - start ) );
            start = end_pos + 1;
            end_pos = sstr.find_first_of( delimiter, start );
        }
        if ( start < sstr.length() )
            list.push_back( sstr.substr( start ) );
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
    // string from vector<string>::iterators
    template <class IT>
    typename std::iterator_traits<IT>::value_type
    StringFromListIT( IT begin, IT end )
    {
        using STR = typename std::iterator_traits<IT>::value_type;
        using CH = typename STR::value_type;

        std::vector<CH>     buffer;

        while ( begin != end )
        {
            buffer.insert( buffer.end(), begin->begin(), begin->end() );
            buffer.push_back( CharConstant<CH>::cr );
            buffer.push_back( CharConstant<CH>::lf );
            ++begin;
        }
        if ( buffer.empty() )
            return STR();
        return STR( &buffer.front(), buffer.size() );
    }

    // string from vector<string>
    template <class CH>
    typename type_helper<CH>::string_type
    Text( const typename type_helper<CH>::list_type& list )
    {
        return StringFromListIT( list.cbegin(), list.cend() );
    }

    // vector from string::iterators
    template <class IT>
    std::vector<typename type_helper<typename std::iterator_traits<IT>::value_type>::string_type>
    ListFromCharIT( IT begin, IT end )
    {
        using CH = typename std::iterator_traits<IT>::value_type;
        using STR = typename type_helper<CH>::string_type;
        using LIST = std::vector<STR>;

        LIST    result;

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
            result.push_back( STR( start, begin ) );
            AdvanceOverCRLF( begin, end, ch );
        }
        return result;
    }

    // vector from string
    template <class CH>
    typename type_helper<CH>::list_type
    Text( const typename type_helper<CH>::string_type& sstr )
    {
        return ListFromCharIT( sstr.cbegin(), sstr.cend() );
    }
#pragma endregion
}

#endif
