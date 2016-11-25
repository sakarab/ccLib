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
            throw cclib::BaseException( boost::str( boost::format( "%1%: library not found." ) % dll_name ) );
    }

    std::string LibLoader::EntryPointNotFound( const char *function_name )
    {
        return boost::str( boost::format( "%1%: entry point not found in %2%." ) % function_name % mDllName.filename() );
    }

}
