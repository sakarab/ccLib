#pragma once

#if ! defined (SCAN_DIR_H)
#define SCAN_DIR_H

#include "IDirIterator.h"
#include <vector>
#include "SharedFlag.h"
#include <boost/function.hpp>

/*********************************************************************
****    ScanDirectory
*********************************************************************/
class ScanDirectory
{
public:
    typedef boost::function<bool ( spFileInfo entry )>      fnObserver;         // returns if used
    typedef boost::function<void ( spFileInfo entry )>      fnNotify;
private:
    typedef std::vector<fnNotify>   NotifyVector;
private:
    fnObserver              mObserver;
    SharedFlag              mFCancel;
    std::vector<fnNotify>   mBeforeFilter;
    std::vector<fnNotify>   mAfterFilter;

    bool check_quit();
    void Notify( std::vector<fnNotify>& vec, spFileInfo file_info );
    void NotifyBefore( spFileInfo file_info )                           { Notify( mBeforeFilter, file_info ); }
    void NotifyAfter( spFileInfo file_info )                            { Notify( mAfterFilter, file_info ); }
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
        : mScan(scan), mPath(path), mFlags(flags)
    {
    }
    void operator()()                       { mScan( mPath, mFlags ); }
    ScanDirectory& ScanDir()                { return mScan; }
    const ScanDirectory& ScanDir() const    { return mScan; }
};

#endif
