//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#pragma once

#if !defined (CC_PRE_CC_WTL_H)
#define CC_PRE_CC_WTL_H

#include "predef_cc.h"

// minimum versions for WTL 10_8356
#define WINVER          0x0600
#define _WIN32_WINNT	0x0600

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#ifndef __BORLANDC__
    #if defined (CC_HAVE_FMT_FORMAT)
        #include <fmt\format.h>
    #else
        #include <boost\format.hpp>
    #endif
    #include <boost/scope_exit.hpp>
#endif

#define _WTL_USE_CSTRING
#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlmisc.h>

#ifndef __BORLANDC__
    #if ( BOOST_LANG_STDCPP == BOOST_VERSION_NUMBER( 41, 12, 1 ) )
        #define CPP_X11_SUPPORT
    #endif
#endif

#endif
