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

#if !defined(PREDEF_CC_H)
#define PREDEF_CC_H

#define BOOST_THREAD_VERSION    3

#ifndef __BORLANDC__
    #include <boost/predef.h>
#endif

#if defined (BOOST_COMP_MSVC_AVAILABLE)

#define CC_CPP_SUPPORT          _MSVC_LANG
#define CC_CPP_SUPPORT_2014     201402L
#define CC_CPP_SUPPORT_2017     201703L

// vs2017
#if ( _MSC_VER >= 1900 )
    #define CC_DEPRICATED_AUTO_PTR
    #define CC_HAVE_ATTRIBUTES
    #define CC_HAVE_FMT_FORMAT
    #define CC_HAVE_UNIFORM_INIT
    #define CC_HAVE_IF_WITH_INIT
    #define CC_HAVE_STRING_VIEW
    #define CC_HAVE_INITIALIZER_LIST
#endif

// vs2013
#if ( _MSC_VER >= 1800 )
    #define CC_HAVE_COPY_DELETE
    #define CC_HAVE_COPY_DEFAULT
    #define CC_HAVE_ATOMIC
    #define CC_HAVE_INLINE_FUNCTORS
    #define CC_HAVE_OVERRIDE
    #define CC_HAVE_ENUM_CLASS
    #define CC_HAVE_RANGE_FOR
    #define CC_HAVE_THREAD
    #define CC_HAVE_VARIADIC_TEMPLATES
    #define CC_HAVE_MAKE_UNIQUE
#endif

// vs2010
#if ( _MSC_VER >= 1600 )
    #define CC_HAVE_MOVE_CTOR
    #define CC_HAVE_NULL_PTR
    #define CC_HAVE_SHARED_PTR
    #define CC_HAVE_ARRAY
#endif

#elif defined (BOOST_COMP_GNUC_AVAILABLE)

    #define CC_DEPRICATED_AUTO_PTR
    #define CC_HAVE_ATTRIBUTES
    #define CC_HAVE_FMT_FORMAT
    #define CC_HAVE_UNIFORM_INIT
    #define CC_HAVE_IF_WITH_INIT
#if __cplusplus >= 201703L
    #define CC_HAVE_STRING_VIEW
#endif
    #define CC_HAVE_INITIALIZER_LIST

    #define CC_HAVE_COPY_DELETE
    //#define CC_HAVE_COPY_DEFAULT
    //#define CC_HAVE_ATOMIC
    #define CC_HAVE_INLINE_FUNCTORS
    #define CC_HAVE_OVERRIDE
    #define CC_HAVE_ENUM_CLASS
    #define CC_HAVE_RANGE_FOR
    #define CC_HAVE_THREAD
    #define CC_HAVE_VARIADIC_TEMPLATES
    //#define CC_HAVE_MAKE_UNIQUE

    #define CC_HAVE_MOVE_CTOR
    #define CC_HAVE_NULL_PTR
    #define CC_HAVE_SHARED_PTR
    #define CC_HAVE_ARRAY

#endif

#if defined(__BORLANDC__)
    // HAS NOTHING
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

#if defined (CC_HAVE_ATTRIBUTES)
    #define CC_USE_ATTRIBUTE(x)     [[x]]
#else
    #define CC_USE_ATTRIBUTE(x)
#endif

#endif
