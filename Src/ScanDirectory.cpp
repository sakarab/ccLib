#include "ScanDirectory.h"
#include <boost/algorithm/string.hpp>

#if defined (QT_CORE_LIB)
#include "IDirIter_Qt.h"
#endif

#include "IDirIter_Boost.h"

namespace fs = boost::filesystem;

/*********************************************************************
****    ScanDirectory
*********************************************************************/
ScanDirectory::ScanDirectory( const fnObserver& observer, const SharedFlag& cancelf )
    : mObserver(observer), mFCancel(cancelf)
{
}

bool ScanDirectory::check_quit()
{
    return mFCancel.is_set();
}

void ScanDirectory::Notify( std::vector<fnNotify>& vec, spFileInfo file_info )
{
    for ( auto it = vec.begin(), eend = vec.end() ; it != eend ; ++it )
        (*it)( file_info );
}

void ScanDirectory::operator()( const boost::filesystem::path& path, IDirIterator::flags flags )
{
    if ( ! mObserver )
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
