#pragma once
#if !defined (CC_MEMORY_HPP)
#define CC_MEMORY_HPP

#include "predef_cc.h"

#if defined (CC_HAVE_SHARED_PTR)
    #include <memory>
    namespace cc
    {
        using std::shared_ptr;
        using std::make_shared;
    }
#else
    #include <boost/smart_ptr.hpp>
    namespace cc
    {
        using boost::shared_ptr;
        using boost::make_shared;
    }
#endif

#endif
