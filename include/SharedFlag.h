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

#if ! defined (SHARED_FLAG_H)
#define SHARED_FLAG_H

#include "cc_atomic.hpp"
#include "cc_memory.hpp"

namespace cclib
{

    /*********************************************************************
    ****    SharedFlag
    *********************************************************************/
    class SharedFlag
    {
    private:
        typedef atomic<bool>             bool_type;
        typedef shared_ptr<bool_type>    sp_bool_type;
    private:
        sp_bool_type        mFlag;
    public:
        SharedFlag() : mFlag( new bool_type )               { set_value( false ); }
        SharedFlag( bool value ) : mFlag( new bool_type )   { set_value( value ); }

        bool is_set() { return *mFlag; }
        void set_value( bool value ) { *mFlag = value; }
    };

}

#endif
