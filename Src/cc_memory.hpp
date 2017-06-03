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

#if !defined (CC_MEMORY_HPP)
#define CC_MEMORY_HPP

#include "predef_cc.h"

namespace cclib
{
    template <class TO, class FROM> TO size_cast( FROM value )          { return static_cast<TO>(value); }

#if defined (CC_HAVE_SHARED_PTR)
    #include <memory>
    using std::shared_ptr;
    using std::make_shared;
#else
    #include <boost/smart_ptr.hpp>
    using boost::shared_ptr;
    using boost::make_shared;
#endif
}

#endif
