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
#include "boDateUtils.h"

namespace ccbo
{
    //======================================================
    //==========    TDateConverter
    //======================================================
    void TDateConverter::construct( const char * format )
    {
        mSStream.exceptions( std::ios_base::failbit );

        boost::gregorian::date_facet        *output_facet = new boost::gregorian::date_facet();
        boost::gregorian::date_input_facet  *input_facet = new boost::gregorian::date_input_facet();

        mSStream.imbue( std::locale( std::locale::classic(), output_facet ) );
        mSStream.imbue( std::locale( mSStream.getloc(), input_facet ) );
        input_facet->format( format );  // stream >> date
        output_facet->format( format );  // stream << date
    }

    TDateConverter::TDateConverter()
    {
        construct( boost::gregorian::date_facet::iso_format_extended_specifier );
    }

    TDateConverter::TDateConverter( const char *format )
    {
        construct( format );
    }

    boost::gregorian::date TDateConverter::to_date( const std::string& ddate )
    {
        mSStream.str( ddate );

        boost::gregorian::date      result;

        mSStream >> result;
        return result;
    }

    std::string TDateConverter::to_string( const boost::gregorian::date& ddate )
    {
        mSStream.str( "" );
        mSStream << ddate;
        return mSStream.str();
    }

    //======================================================
    //==========    TTimeConverter
    //======================================================
    void TTimeConverter::construct( const char * format )
    {
        mSStream.exceptions( std::ios_base::failbit );

        boost::local_time::local_time_facet         *output_facet = new boost::local_time::local_time_facet();
        boost::local_time::local_time_input_facet   *input_facet = new boost::local_time::local_time_input_facet();

        mSStream.imbue( std::locale( std::locale::classic(), output_facet ) );
        mSStream.imbue( std::locale( mSStream.getloc(), input_facet ) );
        input_facet->format( format );  // stream >> date
        output_facet->format( format );  // stream << date
    }

    TTimeConverter::TTimeConverter()
    {
        construct( boost::date_time::time_formats<char>::full_24_hour_time_expanded_format );
    }

    TTimeConverter::TTimeConverter( const char *format )
    {
        construct( format );
    }

    boost::local_time::local_date_time TTimeConverter::to_time( const std::string& time )
    {
        mSStream.str( time );

        boost::local_time::local_date_time      result( boost::date_time::not_a_date_time );

        mSStream >> result;
        return result;
    }

    std::string TTimeConverter::to_string( const boost::local_time::local_date_time& time )
    {
        mSStream.str( "" );
        mSStream << time;
        return mSStream.str();
    }
}