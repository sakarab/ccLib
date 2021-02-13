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

#if !defined (CC_SCOPE_EXIT_H)
#define CC_SCOPE_EXIT_H

#include <predef_cc.h>
#include <boost/preprocessor/cat.hpp>

#if defined(CC_HAVE_INLINE_FUNCTORS)
    #include <functional>
#else
    #include <boost/function.hpp>
#endif


namespace cclib
{
#if defined(CC_HAVE_INLINE_FUNCTORS)
    class scope_exit
    {
    private:
        std::function<void()>   mOnExit;
    public:
        scope_exit( const std::function<void()>& on_exit ) : mOnExit(on_exit)       {}
        ~scope_exit()                                                               { mOnExit(); }
    };

    #define CC_SCOPE_EXIT_IMPL(name,function)       cclib::scope_exit   name(function)
    #define CC_SCOPE_EXIT(a)                        CC_SCOPE_EXIT_IMPL( BOOST_PP_CAT( se_, __LINE__ ), a )
#else
    //template <typename FUNC> struct scope_exit
    //{
    //    FUNC    mFunc;
    //    explicit scope_exit( const FUNC& func ) : mFunc(func)       {};
    //    scope_exit()                                                { mFunc(); }
    //};

    //#define CC_SCOPE_EXIT_IMPL(name,function)       cclib::scope_exit   name(function)
    //#define CC_SCOPE_EXIT(a)                        CC_SCOPE_EXIT_IMPL( BOOST_PP_CAT( se_, __LINE__ ), a )
#endif
}

#endif
