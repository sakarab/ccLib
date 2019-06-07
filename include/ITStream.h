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

#include "predef_cc.h"

namespace cclib
{
    //===========================================================
    //======    ITStream
    //===========================================================
    class ITStream
    {
    public:
        typedef long long       size_type;
    private:
        virtual void Raise_LastError() = 0;
        virtual size_t ReadInternal( void *mem, size_t count ) = 0;
        virtual size_t WriteInternal( const void *mem, size_t count ) = 0;
        virtual size_type get_position() const = 0;
        virtual void set_position( size_type value ) = 0;
        virtual size_type get_size() const = 0;
        virtual void set_size( size_type value ) = 0;
        // noncopyable
        ITStream( const ITStream& src ) CC_EQ_DELETE;
        ITStream& operator = ( const ITStream& src ) CC_EQ_DELETE;
    protected:
        ITStream()                  {} // empty
    public:
        size_type getPosition() const           { return get_position(); }
        void setPosition( size_type value )     { return set_position( value ); }
        size_type getSize() const               { return get_size(); }
        void setSize( size_type value )         { return set_size( value ); }

        size_type CopyFrom( ITStream& src, size_type count );
        void ReadBuffer( void *mem, size_t count );
        void WriteBuffer( const void *mem, size_t count );
        size_t Read( void *mem, size_t count )                  { return ReadInternal( mem, count ); }
        size_t Write( const void *mem, size_t count )           { return WriteInternal( mem, count ); }

        virtual ~ITStream()         {}
    };
}
//namespace cclib
