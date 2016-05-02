#pragma once
#if !defined (CC_PRE_CC_H)
#define CC_PRE_CC_H

#include "predef_cc.h"
#include <string>
#include <vector>
#include <QtCore>

#define _WTL_USE_CSTRING

#if defined (WIN32)
    #include <atlbase.h>
    #include <atlapp.h>
    #include <atlctrls.h>
    #include <atlmisc.h>
#endif

#if ( BOOST_LANG_STDCPP == BOOST_VERSION_NUMBER( 41, 12, 1 ) )
#define CPP_X11_SUPPORT
#endif

#endif
