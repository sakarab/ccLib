//---------------------------------------------------------------------------
#ifndef CPP_STRING_H
#define CPP_STRING_H

#ifndef STD_STRING_DEFINED
#define STD_STRING_DEFINED

#include <string>

#if defined(_UNICODE) || defined(UNICODE)
    typedef std::wstring            std_string;
#else
    typedef std::string             std_string;
#endif
typedef std_string::value_type      std_char;

#endif

#endif
