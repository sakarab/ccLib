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

#include <pre_cc.h>
#include "UntitledSequence.h"
#include <algorithm>

namespace cclib
{

    /********************************************************************
    ********    UntitledSequence
    ********************************************************************/
    UntitledSequence::UntitledSequence()
        : mSequence()
    {}

    UntitledSequence::~UntitledSequence()
    {}

    int UntitledSequence::getAvailable()
    {
        // adjacent diff greater then one (1)
        auto    it = std::adjacent_find( mSequence.begin(), mSequence.end(), []( int arg1, int arg2 ) { return arg2 - arg1 > 1; } );
        int     result = (it != mSequence.end()) ? *it : mSequence.size();

        ++result;
        mSequence.insert( result );
        return result;
    }

    void UntitledSequence::Release( int seq )
    {
        mSequence.erase( seq );
    }

}