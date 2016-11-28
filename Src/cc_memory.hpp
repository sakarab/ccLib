#pragma once
#if !defined (CC_MEMORY_HPP)
#define CC_MEMORY_HPP

#include "predef_cc.h"

template <class TO, class FROM> TO size_cast( FROM value )      { return static_cast<TO>(value); }

#if defined (CC_HAVE_SHARED_PTR)
    #include <memory>
    namespace cclib
    {
        using std::shared_ptr;
        using std::make_shared;
    }
#else
    #include <boost/smart_ptr.hpp>
    namespace cclib
    {
        using boost::shared_ptr;
        using boost::make_shared;
    }
#endif

#endif
