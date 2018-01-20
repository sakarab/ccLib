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

#if ! defined (CC_SCAN_DIR_H)
#define CC_SCAN_DIR_H

#include "IDirIterator.h"
#include <vector>
#include "SharedFlag.h"
#include <boost/function.hpp>

namespace cclib
{

    /*********************************************************************
    ****    ScanDirectory
    *********************************************************************/
    class ScanDirectory
    {
    public:
        typedef boost::function<bool( spFileInfo entry )>      fnObserver;         // returns if used
        typedef boost::function<void( spFileInfo entry )>      fnNotify;
    private:
        typedef std::vector<fnNotify>   NotifyVector;
    private:
        fnObserver              mObserver;
        SharedFlag              mFCancel;
        std::vector<fnNotify>   mBeforeFilter;
        std::vector<fnNotify>   mAfterFilter;

        bool check_quit();
        void Notify( std::vector<fnNotify>& vec, spFileInfo file_info );
        void NotifyBefore( spFileInfo file_info ) { Notify( mBeforeFilter, file_info ); }
        void NotifyAfter( spFileInfo file_info ) { Notify( mAfterFilter, file_info ); }
    public:
        ScanDirectory( const fnObserver& observer, const SharedFlag& cancelf );
        void operator()( const boost::filesystem::path& path, IDirIterator::flags flags );

        void AddNotifyBeforeFilter( const fnNotify& fn );
        void AddNotifyAfterFilter( const fnNotify& fn );
    };

    /*********************************************************************
    ****    ScanDirectoryThreaded
    *********************************************************************/
    class ScanDirectoryThreaded
    {
    private:
        ScanDirectory               mScan;
        boost::filesystem::path     mPath;
        IDirIterator::flags         mFlags;
    public:
        ScanDirectoryThreaded( const ScanDirectory& scan, const boost::filesystem::path& path, IDirIterator::flags flags )
            : mScan( scan ), mPath( path ), mFlags( flags )
        {}
        void operator()() { mScan( mPath, mFlags ); }
        ScanDirectory& ScanDir() { return mScan; }
        const ScanDirectory& ScanDir() const { return mScan; }
    };

}

#endif
