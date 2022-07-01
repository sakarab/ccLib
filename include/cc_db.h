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

#ifndef CC_LIB_DB_H
#define CC_LIB_DB_H

#include <memory>
#include <vector>
#include "cpp_string.h"

namespace ccdb
{
#if defined (CC_HAVE_ENUM_CLASS)
    enum class type { ftBool, ftByte, ftShort, ftInteger, ftLongLong, ftDouble, ftString, ftDate, ftTime, ftDateTime, ftGUID, ftBlob };
#else
	struct type
	{
		enum { ftBool, ftByte, ftShort, ftInteger, ftLongLong, ftDouble, ftString, ftDate, ftTime, ftDateTime, ftGUID, ftBlob };
	};
#endif

    class IDatabase;
    class IStatment;

    typedef std::shared_ptr<IDatabase>      spDatabase;
    typedef std::shared_ptr<IStatment>      spStatment;

    //=======================================================================
    //======    Variant
    //=======================================================================
    class Variant
    {
    };

    typedef std::vector<Variant>    VariantList;

    //=======================================================================
    //======    Param
    //=======================================================================
    class Param
    {
    };

    typedef std::vector<Param>      ParamList;

    //=======================================================================
    //======    Field
    //=======================================================================
    class Field
    {
    public:
        type Type();
        Variant Value();
    };

    //=======================================================================
    //======    IStatment
    //=======================================================================
    class IStatment
    {
    private:
        virtual size_t FieldCount() = 0;
    protected:
        virtual ~IStatment() {}
    public:
        IStatment( const spDatabase& db );
    };

    //=======================================================================
    //======    IDatabase
    //=======================================================================
    class IDatabase
    {
    private:
        virtual void Open( const std_string& connection_string ) = 0;
        virtual void Close() = 0;
        virtual void BeginTransaction() = 0;
        virtual void Commit() = 0;
        virtual void RollBack() = 0;

        virtual spStatment Select( const std_string& sql ) = 0;
        virtual spStatment SelectP( const std_string& sql, const ParamList& params ) = 0;
        virtual spStatment SelectV( const std_string& sql, const VariantList& params ) = 0;
        virtual int Exec( const std_string& sql ) = 0;
        virtual int ExecP( const std_string& sql, const ParamList& params ) = 0;
        virtual int ExecV( const std_string& sql, const VariantList& params ) = 0;
    protected:
        virtual ~IDatabase() {}
    };
}

#endif
