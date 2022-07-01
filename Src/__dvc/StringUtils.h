#pragma once

#include <string>
#include <windows.h>

namespace dvc
{
    bool StrToUInt( const std::wstring& str, UINT& int_result );

    std::string NarrowString( const std::wstring& str );
    std::wstring WidenString( const std::string& str );
    std::wstring BinToHex( const char *bytes, unsigned int length );
}
// namespace dvc
