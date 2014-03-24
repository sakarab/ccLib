#pragma once

#if ! defined (SHARED_FLAG_H)
#define SHARED_FLAG_H

#include <boost/shared_ptr.hpp>
#include <tbb/atomic.h>

/*********************************************************************
****    SharedFlag
*********************************************************************/
class SharedFlag
{
private:
    typedef tbb::atomic<bool>   bool_type;

    boost::shared_ptr<bool_type>    mFlag;
public:
    SharedFlag() : mFlag( new bool_type )                   { set_value( false ); }
    SharedFlag( bool value ) : mFlag( new bool_type )       { set_value( value ); }

    bool is_set()                   { return *mFlag; }
    void set_value( bool value )    { *mFlag = value; }

    //bool operator()()               { return flag(); }
};

#endif
