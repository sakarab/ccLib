#pragma once

#include <Windows.h>
#include <dvc/StringList.h>
#include <dvc/Registry.h>

namespace dvc
{
    class TMachineID
    {
    private:
        TRegistry           mRegWrite;
        TRegistry           mRegRead;
        TRegistry           mIntf;
        TStringList<WCHAR>  mItems;
    public:
    };
}
// namespace dvc
