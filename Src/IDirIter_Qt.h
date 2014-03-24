#pragma once

#if ! defined (IDIRITER_QT_H)
#define IDIRITER_QT_H

#include "IDirIterator.h"
#include <QDirIterator>

/*********************************************************************
****    QT_DirIterator
*********************************************************************/
class QT_DirIterator : public IDirIterator
{
private:
    QDirIterator    mImpl;

    static QDirIterator::IteratorFlag FlagsToQtFlags( IDirIterator::flags flags );
    // noncopyable
    QT_DirIterator( const QT_DirIterator& src );
    QT_DirIterator& operator=( const QT_DirIterator& src );
public:
    QT_DirIterator( const boost::filesystem::path& path, IDirIterator::flags flags );
    // IDirIterator
    virtual bool First();
    virtual bool Next();
    virtual spFileInfo FileInfo();
};

#endif

