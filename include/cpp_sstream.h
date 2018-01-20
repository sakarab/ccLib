//---------------------------------------------------------------------------
#ifndef CPP_SSTREAM_H
#define CPP_SSTREAM_H

#include <sstream>

#if defined(_UNICODE) || defined(UNICODE)
    typedef std::wostringstream     std_ostringstream;
#else
    typedef std::ostringstream      std_ostringstream;
#endif

#endif
