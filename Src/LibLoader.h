#pragma once

#ifndef CCLIB_LIB_LOADER_H
#define CCLIB_LIB_LOADER_H

#include <Windows.h>
#include "predef_cc.h"
#include <boost\format.hpp>
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
