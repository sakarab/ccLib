// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if !defined (CC_PRE_CC_H)
#define CC_PRE_CC_H

#include "predef_cc.h"

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#ifndef __BORLANDC__
    #include <boost/format.hpp>
    #include <boost/scope_exit.hpp>
#endif

#ifndef __BORLANDC__
    #if ( BOOST_LANG_STDCPP == BOOST_VERSION_NUMBER( 41, 12, 1 ) )
        #define CPP_X11_SUPPORT
    #endif
#endif

#endif
