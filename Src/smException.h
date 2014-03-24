#if !defined (SM_EXCEPTION_H_INCLUDED)
#define SM_EXCEPTION_H_INCLUDED

#include <exception>
#include <boost/smart_ptr.hpp>

/**************************************************************************************
************    BaseException
**************************************************************************************/
class BaseException : public std::exception
{
private:
    boost::shared_array<char>   mMessage;
protected:
    void SetMessage( const char *message );
    void SetMessage( const boost::shared_array<char>& message );
public:
    BaseException();
    BaseException( const char *message );
    virtual const char *what() const throw();
};

#endif
