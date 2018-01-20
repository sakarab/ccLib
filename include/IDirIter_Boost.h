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

#if ! defined (IDIRITER_BOOST_H)
#define IDIRITER_BOOST_H

#include <stack>
#include "IDirIterator.h"

/*********************************************************************
****    Boost_DirIterator
*********************************************************************/
class Boost_DirIterator : public IDirIterator
{
private:
    typedef boost::filesystem::directory_iterator       DirIterator;

    class IterPair
    {
    private:
        DirIterator     mIter;
    public:
        IterPair( const DirIterator& iter ) : mIter(iter)               {} // empty
        bool isEnd( const DirIterator& end )                            { return mIter == end; }
        void Advance( const DirIterator& end );

        DirIterator& Iter()                                             { return mIter; }
    };

    std::stack<IterPair>        mStack;
    IDirIterator::flags         mFlags;
    DirIterator                 mEnd;

    bool GoToValid();
    // noncopyable
    Boost_DirIterator( const Boost_DirIterator& src );
    Boost_DirIterator& operator=( const Boost_DirIterator& src );
public:
    Boost_DirIterator( const boost::filesystem::path& path, IDirIterator::flags flags );
    // IDirIterator
    virtual bool First();
    virtual bool Next();
    virtual spFileInfo FileInfo();
};

#endif

