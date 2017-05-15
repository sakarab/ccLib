#pragma once

#include <boost/predef.h>

#if defined (BOOST_COMP_MSVC_AVAILABLE)

// vs2013
#if ( _MSC_VER >= 1800 )
    #define CC_HAVE_COPY_DELETE
    #define CC_HAVE_COPY_DEFAULT
    #define CC_HAVE_ATOMIC
    #define CC_HAVE_INLINE_FUNCTORS
    #define CC_HAVE_OVERRIDE
    #define CC_HAVE_ENUM_CLASS
    #define CC_HAVE_RANGE_FOR
#endif

// vs2010
#if ( _MSC_VER >= 1600 )
    #define CC_HAVE_MOVE_CTOR
    #define CC_HAVE_NULL_PTR
    #define CC_HAVE_SHARED_PTR
    #define CC_HAVE_ARRAY
#endif

#elif defined (BOOST_COMP_GNUC_AVAILABLE)

    #define CC_HAVE_COPY_DELETE
    #define CC_HAVE_ATOMIC
    #define CC_HAVE_INLINE_FUNCTORS
    #define CC_HAVE_OVERRIDE
    #define CC_HAVE_MOVE_CTOR
    #define CC_HAVE_NULL_PTR
    #define CC_HAVE_SHARED_PTR
    #define CC_HAVE_ARRAY

#endif

#if !defined (CC_HAVE_NULL_PTR)
    #define nullptr     NULL
#endif

#if defined(CC_HAVE_COPY_DELETE)
    #define CC_EQ_DELETE    =delete
#else
    #define CC_EQ_DELETE
#endif

#if defined(CC_HAVE_OVERRIDE)
    #define CC_OVERRIDE     override
#else
    #define CC_OVERRIDE
#endif
