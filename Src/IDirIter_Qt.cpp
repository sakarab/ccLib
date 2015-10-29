#include <pre_cc.h>
#include "IDirIter_Qt.h"
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/*********************************************************************
****    QT_FileInfo
*********************************************************************/
class QT_FileInfo : public IFileInfo
{
private:
    typedef boost::shared_ptr<QFileInfo>    spQFileInfo;
private:
    fs::path        mFullName;
    spQFileInfo     mInfo;

    spQFileInfo& GetInfo();
public:
    explicit QT_FileInfo( const QString& full_name );
    explicit QT_FileInfo( const QFileInfo& info );
    explicit QT_FileInfo( const spQFileInfo& info );

    virtual bool isDir();
    virtual const fs::path& fullName();
    virtual long long Size();
};

QT_FileInfo::QT_FileInfo( const QString& full_name )
    : mFullName(full_name.toStdWString()), mInfo()
{
}

QT_FileInfo::QT_FileInfo( const QFileInfo& info )
    : mFullName(info.absolutePath().append( "/" ).append( info.fileName() ).toStdWString()),
      mInfo( spQFileInfo( new QFileInfo( info ) ))
{
}

QT_FileInfo::QT_FileInfo( const spQFileInfo& info )
    : mFullName(info->absolutePath().append( "/" ).append( info->fileName() ).toStdWString()),
      mInfo(info)
{
}

QT_FileInfo::spQFileInfo& QT_FileInfo::GetInfo()
{
    if ( ! mInfo )
        mInfo = spQFileInfo( new QFileInfo( QString::fromStdWString( mFullName.wstring() ) ) );
    return mInfo;
}

bool QT_FileInfo::isDir()                               { return GetInfo()->isDir(); }
const fs::path& QT_FileInfo::fullName()                 { return mFullName; }
long long QT_FileInfo::Size()                           { return GetInfo()->size(); }

/*********************************************************************
****    QT_DirIterator
*********************************************************************/
QT_DirIterator::QT_DirIterator( const boost::filesystem::path& path, IDirIterator::flags flags )
    : mImpl( QString::fromStdWString( path.wstring() ),
             QDir::AllDirs | QDir::Files | QDir::Hidden,
             FlagsToQtFlags( flags ) )
{
}

//static
QDirIterator::IteratorFlag QT_DirIterator::FlagsToQtFlags( IDirIterator::flags flags )
{
    switch ( flags )
    {
        case IDirIterator::idiNoFlags           : return QDirIterator::NoIteratorFlags;                 break;
        case IDirIterator::idiSubdirectories    : return QDirIterator::Subdirectories;                  break;
        case IDirIterator::idiFollowSymlinks    : return QDirIterator::FollowSymlinks;                  break;
        default                                 : throw std::runtime_error( "FlagsToQtFlags error" );   break;  // return QDirIterator::NoIteratorFlags;     break;
    }
}

bool QT_DirIterator::First()
{
    bool    result = mImpl.hasNext();

    if ( result )
        mImpl.next();
    return result;
}

bool QT_DirIterator::Next()
{
    bool    result = mImpl.hasNext();

    if ( result )
        mImpl.next();
    return result;
}

spFileInfo QT_DirIterator::FileInfo()
{
    return spFileInfo( new QT_FileInfo( mImpl.fileInfo() ) );
}
