#pragma hdrstop

#include "ccRegexp.h"
#include <string>

namespace cc
{

const unsigned int maxpat = 128;

__fastcall ccRegexp::ccRegexp()
    : the_pattern(maxpat), stat(OK), FFlags(0)
{
}

__fastcall ccRegexp::ccRegexp( const char *str, int flags )
    : the_pattern(maxpat), stat(OK), FFlags(0)
{
	SetPattern( str, flags );
}

__fastcall ccRegexp::ccRegexp( const ccRegexp &src )
    : the_pattern(src.the_pattern), stat(src.stat), FFlags(src.FFlags)
{
}

__fastcall ccRegexp::~ccRegexp()
{
}

void __fastcall ccRegexp::SetPattern( const char *str, int flags )
{
	char    ss[maxpat];

	FFlags = flags;
	if ( flags & ICASE )
	{
		strcpy( ss, str );
		strupr( ss );
		str = ss;
	}
    the_pattern.resize( 0 );
    the_pattern.resize( maxpat );
	stat = (StatVal)_rtl_makepat( str, &the_pattern.front(), maxpat );
}

size_t __fastcall ccRegexp::find( const char *str, size_t *len, size_t i ) const
{
	char		*startp;
	const char  *s = str;
	const char	*endp = _rtl_matchs( s+i, &the_pattern.front(), &startp );

	if ( endp )
	{
		*len = (size_t)((char  *)endp - startp + 1);
		return (size_t)(startp - (char  *)s);
	}
	else
	{
		*len = 0;
		return -1;
	}
}

};  // namespace cc

