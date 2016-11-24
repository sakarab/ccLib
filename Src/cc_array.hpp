#pragma once
#if !defined (CC_ARRAY_HPP)
#define CC_ARRAY_HPP

#include "predef_cc.h"

#if defined (CC_HAVE_ARRAY)
    #include <array>
    namespace cclib
    {
        using std::array;
    }
#else
    #include <boost/array.hpp>
    namespace cclib
    {
        using boost::array;
    }
#endif

#endif
