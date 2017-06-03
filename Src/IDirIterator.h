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
