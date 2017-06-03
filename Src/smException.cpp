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
#include "smException.h"
#include <cstring>

namespace cclib
{

    /**************************************************************************************
    ************    BaseException
    **************************************************************************************/
    BaseException::BaseException()
        : mMessage()
    {}

    BaseException::BaseException( const char *message )
        : mMessage()
    {
        SetMessage( message );
    }

    BaseException::BaseException( const std::string& message )
        : mMessage()
    {
        SetMessage( message.c_str() );
    }

    const char *BaseException::what() const throw() //override
    {
        if ( !mMessage.get() )
            return "";
        return mMessage.get();
    }

    void BaseException::SetMessage( const char *message )
    {
        const char      *msg = message == nullptr ? "" : message;
        std::size_t     size = std::strlen( msg ) + 1;

        mMessage.reset( new char[size] );
        std::copy( msg, msg + size, mMessage.get() );
    }

    void BaseException::SetMessage( const boost::shared_array<char>& message )
    {
        mMessage = message;
    }

}
//namespace cclib
