#if !defined (SM_EXCEPTION_H_INCLUDED)
#define SM_EXCEPTION_H_INCLUDED

#include <exception>
#include <string>
#include <boost/smart_ptr.hpp>

namespace cc
{

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
    explicit BaseException( const char *message );
    explicit BaseException( const std::string& message );
    virtual const char *what() const throw();
};

}
//namespace cc

#endif
