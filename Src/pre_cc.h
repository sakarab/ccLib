#pragma once

#include <boost/predef.h>
#include <string>
#include <vector>
#include <QtCore>

#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlmisc.h>

#if ( BOOST_LANG_STDCPP == BOOST_VERSION_NUMBER( 41, 12, 1 ) )
#define CPP_X11_SUPPORT
#define DELETE_FUNCTION     = delete
#else
    #if !defined (DELETE_FUNCTION)
        #define DELETE_FUNCTION
    #endif
    #if !defined (nullptr)
        #define nullptr     NULL
    #endif
#endif
