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

#include <pre_cc.h>
#include "LibLoader.h"
#include "win_str.h"

namespace ccwin
{
    /************************************************************
    ********    LibLoader
    ************************************************************/
    LibLoader::LibLoader( const boost::filesystem::path& dll_name )
        : mDllName( dll_name ), mDll()
    {
        LoadLib( dll_name );
    }
    
    //LibLoader::LibLoader( const boost::filesystem::path& dll_name, HMODULE parent_module )
    //    : mDllName(), mDll()
    //{
    //}
    
    LibLoader::~LibLoader()
    {
        if ( mDll )
            FreeLibrary( mDll );
    }

    void LibLoader::LoadLib( const boost::filesystem::path& dll_name )
    {
        mDll = LoadLibrary( dll_name.c_str() );
        if ( mDll == 0 )
#if defined (CC_HAVE_FMT_FORMAT)
            throw cclib::BaseException( fmt::format( "{0}: library not found.", NarrowStringStrict( dll_name.native() ) ) );
#else
            throw cclib::BaseException( boost::str( boost::format( "%1%: library not found." ) % NarrowStringStrict( dll_name.native() ) ) );
#endif
    }

    std::string LibLoader::EntryPointNotFound( const char *function_name )
    {
#if defined (CC_HAVE_FMT_FORMAT)
        return fmt::format( "{0}: entry point not found in {1}.", function_name, NarrowStringStrict( mDllName.filename().native() ) );
#else
        return boost::str( boost::format( "%1%: entry point not found in %2%." ) % function_name % NarrowStringStrict( mDllName.filename().native() ) );
#endif
    }

}
