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

#ifndef UNTITLED_SEQUENCE_H
#define UNTITLED_SEQUENCE_H

#include <memory>
#include <set>
#if _MSVC_LANG < 201703L
#include <loki/Singleton.h>
#endif
#include "predef_cc.h"

namespace cclib
{

    /********************************************************************
    ********    UntitledSequence
    ********************************************************************/
    class UntitledSequence
    {
    private:
        typedef std::set<int>       container;
    private:
#if _MSVC_LANG < 201703L
        friend struct Loki::CreateUsingNew<UntitledSequence>;
#endif
    private:
        container       mSequence;
        // singleton
        UntitledSequence();
        ~UntitledSequence();
        UntitledSequence( const UntitledSequence& ) CC_EQ_DELETE;
        UntitledSequence& operator=( const UntitledSequence& ) CC_EQ_DELETE;
    public:
        int getAvailable();
        void Release( int seq );
    };

    // Instantiate it with a typedef like the following in a cpp file
    // typedef Loki::SingletonHolder<UntitledSequence>     TUntitledSequence;
}

#endif
