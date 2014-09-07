#pragma once

#include <string>
#include <windows.h>

namespace dvc
{
    bool StrToUInt( const std::wstring& str, UINT& int_result );

    std::string NarrowString( const std::wstring& str );
}
// namespace dvc
