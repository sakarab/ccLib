#pragma once

#if ! defined (SHARED_FLAG_H)
#define SHARED_FLAG_H

#include <atomic>
#include <memory>

/*********************************************************************
****    SharedFlag
*********************************************************************/
class SharedFlag
{
private:
    typedef std::atomic<bool>   bool_type;
private:
    std::shared_ptr<bool_type>  mFlag;
public:
    SharedFlag() : mFlag( new bool_type )                   { set_value( false ); }
    SharedFlag( bool value ) : mFlag( new bool_type )       { set_value( value ); }

    bool is_set()                   { return *mFlag; }
    void set_value( bool value )    { *mFlag = value; }
};

#endif
