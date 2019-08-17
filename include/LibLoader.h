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

#ifndef CCLIB_LIB_LOADER_H
#define CCLIB_LIB_LOADER_H

#include <Windows.h>
#include "predef_cc.h"
#include <boost\filesystem.hpp>
#include "smException.h"
#include "cpp_string.h"

namespace ccwin
{

    /************************************************************
    ********    LibLoader
    ************************************************************/
    class LibLoader
    {
    private:
        HMODULE                     mDll;
        boost::filesystem::path     mDllName;

        void LoadLib( const boost::filesystem::path& dll_name );
        std::string EntryPointNotFound( const char *function_name );

        // noncopyable
        LibLoader( const LibLoader& src ) CC_EQ_DELETE;
        LibLoader& operator = ( const LibLoader& src ) CC_EQ_DELETE;
    public:
        LibLoader( const boost::filesystem::path& dll_name );
        //LibLoader( const boost::filesystem::path& dll_name, HMODULE parent_module );
        ~LibLoader();

        template <class T> T GetFuncAddress( const char *function_name )
        {
            T       result = reinterpret_cast<T>(GetProcAddress( mDll, function_name ));

            if ( result == nullptr )
                throw cclib::BaseException( EntryPointNotFound( function_name ) );
            return result;
        }
    };

}

#endif
