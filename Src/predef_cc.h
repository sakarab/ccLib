#pragma once

#include <boost/predef.h>

#if defined (BOOST_COMP_MSVC_AVAILABLE)

#if ( BOOST_COMP_MSVC >= BOOST_VERSION_NUMBER(12,0,0) )
    #define HAVE_COPY_DELETE
#endif

#if ( BOOST_COMP_MSVC >= BOOST_VERSION_NUMBER(10,0,0) )
    #define HAVE_MOVE_CTOR
#endif

#if defined(HAVE_COPY_DELETE)
    #define CC_COPY_DELETE  =delete
#else
    #define CC_COPY_DELETE
#endif

#else

#define CC_COPY_DELETE

#endif
