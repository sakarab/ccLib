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
