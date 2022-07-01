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

#if defined(CPP_STRINGLIST_H)
    #pragma message("It is best not to #include both 'cpp_StringList.h' and 'cpp_string_list.h'.")
#endif

#include <string>
#include <vector>
#include <cpp_string.h>

namespace cclib
{
    template <class CH> struct type_helper
    {
        typedef std::basic_string<CH, std::char_traits<CH>, std::allocator<CH> >    string_type;
#ifdef CC_HAVE_STRING_VIEW
        typedef std::basic_string_view<CH, std::char_traits<CH> >                 string_view_type;
#else
        typedef boost::basic_string_view<CH, std::char_traits<CH> >               string_view_type;
#endif
    };

#pragma region ToString
    template <class IT>
    typename std::iterator_traits<IT>::value_type
    ToString( IT begin, IT end )
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

    template <class IT>
    typename std::iterator_traits<IT>::value_type
    ToString( IT begin, IT end, typename std::iterator_traits<IT>::value_type::value_type ch )
    {
        using STR = typename std::iterator_traits<IT>::value_type;
        using CH = typename STR::value_type;

        std::vector<CH>     buffer;

        while ( begin != end )
        {
            buffer.insert( buffer.end(), begin->begin(), begin->end() );
            buffer.push_back( ch );
            ++begin;
        }
        if ( buffer.empty() )
            return STR();
        return STR( &buffer.front(), buffer.size() - 1 );
    }

    // string from vector<string>
    template <class CH>
    typename CH::value_type
    ToString( const CH& list )
    {
        return ToString( list.cbegin(), list.cend() );
    }

    template <class CH>
    typename CH::value_type
    ToString( const CH& list, typename CH::value_type::value_type delimiter )
    {
        return ToString( list.cbegin(), list.cend(), delimiter );
    }

#pragma endregion

#pragma region ToList from char iterators
    template <typename LIST, typename IT>
    LIST ToList( IT begin, IT end )
    {
        using CH = typename LIST::value_type::value_type;
        using STR = typename LIST::value_type;

        LIST        result;

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

    template <typename LIST, typename IT>
    LIST ToList( IT begin, IT end, typename LIST::value_type::value_type delimiter )
    {
        using STR = typename LIST::value_type;

        LIST        result;

        while ( begin != end )
        {
            IT      start = begin;

            while ( *begin != delimiter )
            {
                ++begin;
                if ( begin == end )
                    break;
            }
            result.push_back( STR( start, begin ) );
            if ( begin != end )
                ++begin;
        }
        return result;
    }

    template <typename LIST>
    LIST ToList( const typename type_helper<typename LIST::value_type::value_type>::string_type& sstr )
    {
        return ToList<LIST>( sstr.begin(), sstr.end() );
    }

    template <class LIST>
    LIST ToList( const typename type_helper<typename LIST::value_type::value_type>::string_type& sstr,
                    typename LIST::value_type::value_type delimiter )
    {
        return ToList<LIST>( sstr.begin(), sstr.end(), delimiter );
    }

    template <class LIST>
    LIST ToList( const typename type_helper<typename LIST::value_type::value_type>::string_view_type& sstr )
    {
        return ToList<LIST>( sstr.begin(), sstr.end() );
    }

    template <class LIST>
    LIST ToList( const typename type_helper<typename LIST::value_type::value_type>::string_view_type& sstr,
                    typename LIST::value_type::value_type delimiter )
    {
        return ToList<LIST>( sstr.begin(), sstr.end(), delimiter );
    }
#pragma endregion

}

#endif
