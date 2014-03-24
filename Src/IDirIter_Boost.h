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

