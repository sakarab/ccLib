#pragma once

#if ! defined (SHARED_FLAG_H)
#define SHARED_FLAG_H

#if defined (CPP_X11_SUPPORT)
    #include <atomic>
#else
    #include <boost/atomic.hpp>
#endif
#include <memory>

/*********************************************************************
****    SharedFlag
*********************************************************************/
class SharedFlag
{
private:
#if defined (CPP_X11_SUPPORT)
    typedef std::atomic<bool>               bool_type;
    typedef std::shared_ptr<bool_type>      sp_bool_type;
#else
    typedef boost::atomic<bool>             bool_type;
    typedef boost::shared_ptr<bool_type>    sp_bool_type;
#endif
private:
    sp_bool_type        mFlag;
public:
    SharedFlag() : mFlag( new bool_type )                   { set_value( false ); }
    SharedFlag( bool value ) : mFlag( new bool_type )       { set_value( value ); }

    bool is_set()                   { return *mFlag; }
    void set_value( bool value )    { *mFlag = value; }
};

#endif
