#pragma once
#if !defined (CC_ATOMIC_HPP)
#define CC_ATOMIC_HPP

#include "predef_cc.h"

#if defined (CC_HAVE_ATOMIC)
    #include <atomic>
    namespace cc
    {
        using std::atomic;
    }
#else
    #include <boost/atomic.hpp>
    namespace cc
    {
        using boost::atomic;
    }
#endif

#endif
