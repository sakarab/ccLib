#include <pre_cc.h>
#include "LibLoader.h"
#include "win_str.h"

namespace ccwin
{
    /************************************************************
    ********    LibLoader
    ************************************************************/
    LibLoader::LibLoader( const std::wstring& dll_name )
        : mDllName( dll_name ), mDll( 0 )
    {
    }
    
    LibLoader::LibLoader( const std::wstring& dll_name, HMODULE parent_module )
        : mDllName( dll_name ), mDll( parent_module )
    {
    }
    
    LibLoader::~LibLoader()
    {
    }

    void LibLoader::LoadLib( const std::wstring & dll_name )
    {
        mDll = LoadLibrary( dll_name.c_str() );
        if ( mDll == 0 )
            throw cclib::BaseException( boost::str( boost::format( "%1%: library not found." ) % NarrowStringStrict( dll_name ) ) );
    }

    std::string LibLoader::EntryPointNotFound( const char *function_name )
    {
        return boost::str( boost::format( "%1%: entry point not found." ) % function_name );
    }

}
