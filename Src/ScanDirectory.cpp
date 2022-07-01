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
#include "ScanDirectory.h"
#include <boost/algorithm/string.hpp>
#include "IDirIter_Boost.h"

namespace fs = boost::filesystem;

namespace cclib
{

    /*********************************************************************
    ****    ScanDirectory
    *********************************************************************/
    ScanDirectory::ScanDirectory( const fnObserver& observer, const SharedFlag& cancelf )
        : mObserver( observer ), mFCancel( cancelf )
    {}

    bool ScanDirectory::check_quit()
    {
        return mFCancel.is_set();
    }

    void ScanDirectory::Notify( std::vector<fnNotify>& vec, spFileInfo file_info )
    {
        for ( NotifyVector::iterator it = vec.begin(), eend = vec.end() ; it != eend ; ++it )
            (*it)(file_info);
    }

    void ScanDirectory::operator()( const boost::filesystem::path& path, IDirIterator::flags flags )
    {
        if ( !mObserver )
            return;

        //boost::shared_ptr<IDirIterator>     dit( new QT_DirIterator( path, flags ) );
        boost::shared_ptr<IDirIterator>     dit( new Boost_DirIterator( path, flags ) );

        if ( dit->First() )
            do
            {
                if ( check_quit() )
                    break;

                spFileInfo  file_info = dit->FileInfo();

                NotifyBefore( file_info );
                if ( mObserver( file_info ) )
                    NotifyAfter( file_info );
            }
        while ( dit->Next() );
    }

    void ScanDirectory::AddNotifyBeforeFilter( const fnNotify& fn )
    {
        mBeforeFilter.push_back( fn );
    }

    void ScanDirectory::AddNotifyAfterFilter( const fnNotify& fn )
    {
        mAfterFilter.push_back( fn );
    }

}