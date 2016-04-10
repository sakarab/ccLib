#pragma once
#if !defined (CC_ARRAY_HPP)
#define CC_ARRAY_HPP

#include "predef_cc.h"

#if defined (CC_HAVE_ARRAY)
    #include <array>
    namespace cc
    {
        using std::array;
    }
#else
    #include <boost/array.hpp>
    namespace cc
    {
        using boost::array;
    }
#endif

#endif
