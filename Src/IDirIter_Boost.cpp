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
#include "IDirIter_Boost.h"

namespace fs = boost::filesystem;

/*********************************************************************
****    Boost_FileInfo
*********************************************************************/
class Boost_FileInfo : public IFileInfo
{
private:
    fs::directory_entry     mDirEntry;
public:
    Boost_FileInfo( const fs::directory_entry& dir_entry );

    virtual bool isDir();
    virtual const fs::path& fullName();
    virtual long long Size();
};

Boost_FileInfo::Boost_FileInfo( const fs::directory_entry& dir_entry )
    : mDirEntry(dir_entry)
{
}

bool Boost_FileInfo::isDir()                { return fs::is_directory( mDirEntry ); }
const fs::path& Boost_FileInfo::fullName()  { return mDirEntry.path(); }
long long Boost_FileInfo::Size()            { return fs::file_size( mDirEntry ); }

/*********************************************************************
****    Boost_DirIterator::IterPair
*********************************************************************/
void Boost_DirIterator::IterPair::Advance( const DirIterator& end )
{
    bool    error = false;

    do
    {
        try
        {
            error = false;
            ++mIter;
        }
        catch ( const boost::filesystem::filesystem_error& )
        {
            error = true;
        }
    }
    while ( error && !isEnd( end ) );
}

/*********************************************************************
****    Boost_DirIterator
*********************************************************************/
Boost_DirIterator::Boost_DirIterator( const boost::filesystem::path& path, IDirIterator::flags flags )
    : mStack(), mFlags(flags), mEnd()
{
    mStack.push( IterPair( DirIterator( path ) ) );
}

bool Boost_DirIterator::GoToValid()
{
    IterPair&   iter = mStack.top();
    bool        result = ! iter.isEnd( mEnd );

    while ( ! result && mStack.size() > 1 )
    {
        mStack.pop();
        mStack.top().Advance( mEnd );
        result = GoToValid();
    }
    return result;
}

bool Boost_DirIterator::First()
{
    // First() has taken place on construction
    return ! mStack.top().isEnd( mEnd );
}

bool Boost_DirIterator::Next()
{
    IterPair&   iter = mStack.top();
    bool        result = false;

    if ( ! iter.isEnd( mEnd ) )
    {
        if ( mFlags == IDirIterator::idiSubdirectories && boost::filesystem::is_directory( *iter.Iter() ) )
        {
            try
            {
                mStack.push( IterPair( DirIterator( boost::filesystem::wpath( iter.Iter()->path().wstring() ) ) ) );
                result = true;
            }
            catch ( const boost::filesystem::filesystem_error& )
            {
                iter.Advance( mEnd );
                result = GoToValid();
            }
        }
        else
        {
            iter.Advance( mEnd );
            result = GoToValid();
        }
    }
    return result;
}

spFileInfo Boost_DirIterator::FileInfo()
{
    GoToValid();
    return spFileInfo( new Boost_FileInfo( *mStack.top().Iter() ) );   //QFileInfo( wstringToQString( mStack.top().Iter()->path().wstring() ) );
}
