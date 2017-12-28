#pragma once

#ifndef CCLIB_CPP_LPSTR_H
#define CCLIB_CPP_LPSTR_H

#include <string>
#include <boost/smart_ptr.hpp>

namespace cclib
{
    boost::shared_array<std::string::value_type> LPSTR( const std::string& str );
    boost::shared_array<std::wstring::value_type> LPSTR( const std::wstring& str );
}

#endif
