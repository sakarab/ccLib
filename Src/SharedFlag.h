#pragma once

#if ! defined (SHARED_FLAG_H)
#define SHARED_FLAG_H

#include "cc_atomic.hpp"
#include "cc_memory.hpp"

namespace cclib
{

    /*********************************************************************
    ****    SharedFlag
    *********************************************************************/
    class SharedFlag
    {
    private:
        typedef atomic<bool>             bool_type;
        typedef shared_ptr<bool_type>    sp_bool_type;
    private:
        sp_bool_type        mFlag;
    public:
        SharedFlag() : mFlag( new bool_type )               { set_value( false ); }
        SharedFlag( bool value ) : mFlag( new bool_type )   { set_value( value ); }

        bool is_set() { return *mFlag; }
        void set_value( bool value ) { *mFlag = value; }
    };

}

#endif
