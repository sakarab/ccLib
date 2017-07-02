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

#ifndef CCLIB_BO_DATE_UTILS_H
#define CCLIB_BO_DATE_UTILS_H

#pragma once

#include <string>
#include <iostream>
#include <boost/date_time/local_time/local_time.hpp>

namespace ccbo
{
    //======================================================
    //==========    TDateConverter
    //======================================================
    class TDateConverter
    {
    private:
        std::stringstream           mSStream;

        void construct( const char *format );
    public:
        TDateConverter();
        TDateConverter( const char *format );

        boost::gregorian::date to_date( const std::string& ddate );
        std::string to_string( const boost::gregorian::date& ddate );
    };

    // TDateConverter   GR_Date( "%d/%m/%Y" );
    // TDateConverter   XML_Date( boost::gregorian::date_facet::iso_format_extended_specifier );

    //======================================================
    //==========    TTimeConverter
    //======================================================
    class TTimeConverter
    {
    private:
        std::stringstream           mSStream;

        void construct( const char *format );
    public:
        TTimeConverter();
        TTimeConverter( const char *format );

        boost::local_time::local_date_time to_time( const std::string& time );
        std::string to_string( const boost::local_time::local_date_time& time );
    };
}
#endif
