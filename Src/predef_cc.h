#pragma once

#include <boost/predef.h>

#if defined (BOOST_COMP_MSVC_AVAILABLE)

#if ( BOOST_COMP_MSVC_EMULATED >= BOOST_VERSION_NUMBER(12,0,0) )
    #define CC_HAVE_COPY_DELETE
    #define CC_HAVE_ATOMIC
#endif

#if ( BOOST_COMP_MSVC_EMULATED >= BOOST_VERSION_NUMBER(10,0,0) )
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

#else

#define CC_EQ_DELETE

#endif
