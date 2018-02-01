#ifndef __CCREGEXP_H
#define __CCREGEXP_H

#include <vector>

/*----------------------------------------------------------------------*/
/*																		*/
/*  class ccRegexp														*/
/*																		*/
/*  Implements regular expression searching								*/
/*																		*/
/*----------------------------------------------------------------------*/

namespace cc
{

class ccRegexp
{
public:
	enum StatVal { OK = 0, ILLEGAL, NOMEMORY, TOOLONG };
	enum Flags { ICASE = 0x01 };
private:
	typedef unsigned char	PatternType;
	typedef PatternType		pattern;	/* pattern strings are unsigned char */

	std::vector<unsigned char>  the_pattern;
	StatVal					    stat;
	int						    FFlags;

	int			__fastcall	_rtl_makepat( const char *exp, pattern *pat, size_t maxpat );
	const char *__fastcall	_rtl_matchs( const char *str, const pattern *pat, char **startpat ) const;
	pattern		__fastcall	esc( const char **s );
	const char *__fastcall	doccl( pattern *map, const char *src );
	int			__fastcall	omatch( const char **strp, const pattern *pat, const char *start ) const;
	const char *__fastcall	patcmp( const char *str, const pattern *pat, const char *start ) const;
	inline void __fastcall	SETBIT( pattern b, pattern *map );
	inline bool __fastcall	TSTBIT( pattern b, const pattern *map ) const;
public:
	__fastcall ccRegexp();
	__fastcall ccRegexp( const char *cp, int flags = 0 );
	__fastcall ccRegexp( const ccRegexp &src );
	__fastcall ~ccRegexp();

	ccRegexp& __fastcall operator = ( const char *cp ) throw();
	ccRegexp& __fastcall operator = ( const ccRegexp &src ) throw();

	void __fastcall SetPattern( const char *str, int flags = 0 );

	size_t __fastcall  find( const char *cp, size_t *len, size_t start = 0 ) const;
	StatVal __fastcall  status()															{ return stat; }
};

};  // namespace cc

#if !defined(RC_INVOKED)

#if defined(__STDC__)
#pragma warn .nak
#endif

#endif  /* !RC_INVOKED */

#endif /* __REGEXP_H */

