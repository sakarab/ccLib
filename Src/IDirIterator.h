#pragma once

#if ! defined (IDIRITERATOR_H)
#define IDIRITERATOR_H

#include <string>
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>

/*********************************************************************
****    IFileInfo
*********************************************************************/
class IFileInfo
{
public:
    virtual bool isDir() = 0;
    virtual const boost::filesystem::path& fullName() = 0;
    virtual long long Size() = 0;

    virtual ~IFileInfo()            {} // empty
};

typedef boost::shared_ptr<IFileInfo>    spFileInfo;

/*********************************************************************
****    IDirIterator
*********************************************************************/
class IDirIterator
{
public:
    enum flags { idiNoFlags, idiSubdirectories, idiFollowSymlinks };
public:
    virtual bool First() = 0;
    virtual bool Next() = 0;
    virtual spFileInfo FileInfo() = 0;

    virtual ~IDirIterator()         {} // empty
};


#endif

