#include "smException.h"

/**************************************************************************************
************    BaseException
**************************************************************************************/
BaseException::BaseException()
    : mMessage()
{
}

BaseException::BaseException( const char *message )
    : mMessage()
{
    SetMessage( message );
}

//virtual
const char *BaseException::what() const throw()
{
    if ( !mMessage.get() )
        return "";
    return mMessage.get();
}

void BaseException::SetMessage( const char *message )
{
    std::size_t     size = std::strlen( message ) + 1;

    mMessage.reset( new char[size] );
    std::copy( message, message + size, mMessage.get() );
}

void BaseException::SetMessage( const boost::shared_array<char>& message )
{
    mMessage = message;
}
