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
