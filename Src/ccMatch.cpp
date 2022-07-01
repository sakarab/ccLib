/*
 * Author:    Allen I. Holub
 *
 * (c) C Gazette. May be used freely as long as author and publication are acknowledged
 */

/*
 * Modified by Borland International to compile without warnings as C++.
 */

#pragma hdrstop

#include <_defs.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ccRegexp.h"
#if defined(_MBCS)
#include <mbctype.h>
#endif

#undef	max
#define	max(a,b)		(((a) > (b)) ? (a) : (b))

/* Metacharacters in the input:         */
#define	BOL			'^'		/* start-of-line anchor					*/
#define	EOL			'$'		/* end-of-line anchor					*/
#define	ANY			'.'		/* matches any character				*/
#define	CCL			'['		/* start a character class				*/
#define	CCLEND		']'		/* end a character class				*/
#define	NCCL		'^'		/* negates character class if 1st char.	*/
#define	CLOSURE		'*'		/* Kleene closure (matches 0 or more)	*/
#define	PCLOSE		'+'		/* Positive closure (1 or more)			*/
#define	OPT			'?'		/* Optional closure (0 or 1)			*/

namespace cc
{

/* These are put in the pattern string to represent metacharacters.         */
typedef enum action
{
#if defined(_MBCS)
  M_NOTASCII = 0x81,
#endif
  M_BOL =    (0x80 | '^'),
  M_EOL =    (0x80 | '$'),
  M_ANY =    (0x80 | '.'),
  M_CCL =    (0x80 | '['),
  M_OPT =    (0x80 | '?'),
  M_CLOSE =  (0x80 | '*'),
  M_PCLOSE = (0x80 | '+')
} action;

#define IS_ACTION(x)	((x)&0x80)	/* true => element of pat. string is an   */
									/* action that represents a metacharacter */

/*----------------------------------------------------------------------*/
#define MAPSIZE 16		/* need this many bytes for character class bit map */

/*
 * Advance a pointer into the pattern template to the next pattern element, this is a +1 for
 * all pattern elements but M_CCL, where you to skip past both the M_CCL character and the
 * bitmap that follows that character
 */

#if defined(_MBCS)
	#define ADVANCE(pat)	skipapat(&pat)
#else
	#define ADVANCE(pat)	(pat += (*pat == (pattern)M_CCL) ? (MAPSIZE+1) : 1)
#endif

/*----------------------------------------------------------------------*/
#define E_NONE		0		/* Possible return values from pat_err. */
#define E_ILLEGAL	1		/* Set in _rtl_makepat() to indicate prob- */
#define E_NOMEM		2		/* lems that came up while making the   */
#define E_PAT		3		/* pattern template.                    */
/*----------------------------------------------------------------------*/

static int	hex2bin( int );
static int	oct2bin( int );

#if defined(_MBCS)
static const pattern	*skipapat(const pattern * *);
#endif

/* Bitmap functions. Set bit b in the map and test bit b to see if it was set previously. */
inline void __fastcall ccRegexp::SETBIT( pattern b, pattern *map )
{
	map[((b & 0x7f) >> 3)] |= (pattern)(1 << (b & 0x07));
}

inline bool __fastcall ccRegexp::TSTBIT( pattern b, const pattern *map ) const
{
	return ( (map[((b & 0x7f) >> 3)] & (1 << (b & 0x07))) != 0 );
}

/*----------------------------------------------------------------------*/
  /*
   * Make a pattern template from the string pointed to by exp. Stop when '\0' or '\n' is found in exp.
   *  The pattern template is assembled in pat whose length is given by maxpat.
   *
   * Return:
   * E_ILLEGAL       Illegal input pattern.
   * E_NOMEM         out of memory.
   * E_PAT           pattern too long.
   */

//  char           *exp;      /* Regular expression */
//  pattern        *pat;      /* Assembled compiled pattern */
//  int             maxpat;   /* Length of pat */
int __fastcall ccRegexp::_rtl_makepat( const char *exp, pattern *pat, size_t maxpat )
{
	pattern		*cur;				/* pointer to current pattern element  */
	pattern		*prev;				/* pointer to previous pattern element */
	int			Error = E_ILLEGAL;

	if ( ! *exp || *exp == '\n')
		goto exit;
	if ( *exp == CLOSURE || *exp == PCLOSE || *exp == OPT )
		goto exit;

	Error = E_NOMEM;
	if ( ! pat )
		goto exit;      /* Check for bad pat */

	prev = cur = pat;
	Error = E_PAT;

	while ( *exp && *exp != '\n' )
	{
		if ( cur >= &pat[maxpat - 1] )
			goto exit;
		switch ( *exp )
		{
			case ANY:
				*cur = (pattern)M_ANY;
				prev = cur++;
				++exp;
				break;
			case BOL:
				*cur = (cur == pat) ? (pattern)M_BOL : *exp;
				prev = cur++;
				++exp;
				break;
			case EOL:
				*cur = (!exp[1] || exp[1] == '\n') ? (pattern)M_EOL : *exp;
				prev = cur++;
				++exp;
				break;
			case CCL:
				if ( (size_t)((cur - pat) + MAPSIZE) >= maxpat )
					goto exit;      /* not enough room for bit map */
				prev = cur;
				*cur++ = (pattern)M_CCL;
				exp = doccl(cur, exp);
				cur += MAPSIZE;
				break;
			case OPT:
			case CLOSURE:
			case PCLOSE:
				switch ( *prev )
				{
					case M_BOL:
					case M_EOL:
					case M_OPT:
					case M_PCLOSE:
					case M_CLOSE:
						goto exit;
				}
				// memmove( prev+1, prev, cur-prev );
				{
					pattern		*p = cur;

					while ( p > prev )
					{
						*p = *(p - 1);
						p--;
					}
				}
				*prev = (*exp == OPT) ? (pattern)M_OPT : (*exp == PCLOSE) ? (pattern)M_PCLOSE : (pattern)M_CLOSE;
				++cur;
				++exp;
				break;
			default:
				prev = cur;
#if defined(_MBCS)
				pattern		c = esc(&exp);

				if ( c > 0x7F )
				{
					if ( cur >= &pat[maxpat - 2] )
						goto exit;
					*cur++ = M_NOTASCII;
					*cur++ = (pattern)(c - 0x7F);
					if ( _ismbblead(c) && *exp )
					{
						c = *exp++;
						if ( cur >= &pat[maxpat - ((c > 0x7F) ? 2 : 1)] )
							goto exit;
						if ( c > 0x7F )
						{
							*cur++ = M_NOTASCII;
							*cur++ = (pattern)(c - 0x7F);
						}
						else
							*cur++ = c;
					}
				}
				else
					*cur++ = c;
#else
				*cur++ = esc(&exp);
#endif
				break;
		}
	}
	*cur = '\0';
	Error = E_NONE;
exit:
	return Error;
}

/*----------------------------------------------------------------------*/
/*
 * Set bits in the map corresponding to characters specified in the src character class.
 */

const char * __fastcall ccRegexp::doccl( pattern *map, const char *src )
{
	int			first, last, val;
	bool		negative;

	++src;												/* skip past the [          */
	if ( (negative = (*src == NCCL)) != false )			/* check for negative ccl   */
		++src;
	memset( map, 0, MAPSIZE );							/* bitmap initially empty       */
	val = esc( &src );

	while ( *src )
	{
		if ( val == CCLEND )
			break;
		else if ( val == '-' )
			SETBIT( '-', map );							/* literal dash at start or end */
		else if ( *src == '-' )
		{
			++src;										/* skip to end-of-sequence char */
			last = esc( &src );
			if ( last < val )
			{
				first = last;
				last = val;
			}
			else
				first = val;
			first--;
			while ( ++first <= last )
				SETBIT( (pattern)first, map );
		}
		else
			SETBIT( (pattern)val, map );
		val = esc( &src );
	}
	if ( negative )
		for ( first = MAPSIZE ; --first >= 0 ;  )
			*map++ ^= ~0;								/* invert all bits */
	return src;
}

/*----------------------------------------------------------------------*/
const char * __fastcall ccRegexp::_rtl_matchs( const char *str, const pattern *pat, char **startpat ) const
{
	const char		*endp = NULL, *start;

	if ( !pat )
		return NULL;
	if ( *str == '\0' )
	{
		if ( (*pat == (pattern)M_EOL) || (*pat == (pattern)M_BOL && (!pat[1] || pat[1] == (pattern)M_EOL)) )
			endp = str;
	}
	else
	{
		start = str;	/* Do a brute-force substring search, comparing a pattern against the input string */
		while ( *str )
		{
			endp = patcmp( str, pat, start );
			if ( endp )
				break;
#if defined(_MBCS)
			if ( _ismbblead(*str) && str[1] )
				str += 2;
			else
				str++;
#else
			str++;
#endif
		}
	}
	*startpat = (char *)str;
	return endp;
}

/*----------------------------------------------------------------------*/
/*
 * Like strcmp, but compares str against pat. Each element of str is compared with the template until
 * either a mis-match is found or the end of the template is reached. In the former case a 0 is returned;
 * in the latter, a pointer into str (pointing to the last character in the matched pattern) is returned.
 * Strstart points at the first character in the string, which might not be the same thing as line
 * if the search started in the middle of the string.
 */

const char * __fastcall ccRegexp::patcmp( const char *str, const pattern *pat, const char *start ) const
{
	const char		*bocl;		/* beginning of closure string.         */
	const char		*end=0;		/* return value: end-of-string pointer. */

	if ( !pat )					/* make sure pattern is valid   */
		return (NULL);

	while ( *pat )
	{
		if ( *pat == (pattern)M_OPT )
		{
			/*
			 * Zero or one matches. It doesn't matter if omatch fails---it will
			 * advance str past the character on success, though. Always advance
			 * the pattern past both the M_OPT and the operand.
			 */

			omatch( &str, ++pat, start );
			ADVANCE(pat);
		}
		else if ( !(*pat == (pattern)M_CLOSE || *pat == (pattern)M_PCLOSE) )
		{
			/*
			 * Do a simple match. Note that omatch() fails if there's still
			 * something in pat but we're at end of string.
			 */

			if ( ! omatch( &str, pat, start ) )
				return NULL;
			ADVANCE(pat);
		}
		else
		{					/* Process a Kleene or positive closure */
			if ( *pat++ == (pattern)M_PCLOSE )		/* one match required */
				if ( ! omatch( &str, pat, start ) )
					return NULL;

			/* Match as many as possible, zero is okay */
			bocl = str;
			while ( *str && omatch( &str, pat, start ) )
				;	/* do nothing */

			/*
			 * 'str' now points to the character that made made us fail. Try to
			 * process the rest of the string. If the character following the
			 * closure could have been in the closure (as in the pattern "[a-z]*t")
			 * the final 't' will be sucked up in the while loop. So, if the match
			 * fails, back up a notch and try to match the rest of the string
			 * again, repeating this process recursively until we get back to the
			 * beginning of the closure. The recursion goes, at most, one levels
			 * deep.
			 */

			if ( *ADVANCE(pat) )
			{
				for (  ; bocl <= str ; --str )
				{
					end = patcmp( str, pat, start );
					if ( end )
						break;
				}
				return end;
			}
			break;
		}
	}

	/*
	 * omatch() advances str to point at the next character to be matched. So
	 * str points at the character following the last character matched when
	 * you reach the end of the template. The exceptions are templates
	 * containing only a BOLN or EOLN token. In these cases omatch doesn't
	 * advance. Since we must return a pointer to the last matched character,
	 * decrement str to make it point at the end of the matched string, making
	 * sure that the decrement hasn't gone past the beginning of the string.
	 *
	 * Note that $ is a position, not a character, but in the case of a pattern
	 * ^$, a pointer to the end of line character is returned. In ^xyz$, a
	 * pointer to the z is returned.
	 *
	 * The --str is done outside the return statement because max() is a macro
	 * with side-effects.
	 */

	--str;
	return ( max(start, str) );
}

static inline char __fastcall read_char( const char *str, int flags )
{
	if ( flags & ccRegexp::ICASE )
		return ( (char)toupper( *str ) );
	else
		return ( *str );
}

/*----------------------------------------------------------------------*/
  /*
   * Match one pattern element, pointed at by pat, against the character at
   * **strp. Return 0 on a failure, 1 on success. *strp is advanced to skip
   * over the matched character on a successful match. Closure is handled one
   * level up by patcmp().
   *
   * "start" points at the character at the left edge of the line. This might
   * not be the same thing as *strp if the search is starting in the middle
   * of the string. An end-of- line anchor matches '\n' or '\0'.
   */
int __fastcall ccRegexp::omatch( const char **strp, const pattern *pat, const char *start ) const
{
	int		advance = -1;		/* amount to advance *strp, -1 == error  */

	switch ( *pat )
	{
		case M_BOL :					/* First char in string? */
			if ( *strp == start )		/* Only one star here.   */
				advance = 0;
			break;
		case M_ANY :					/* . = anything but newline */
			if ( **strp != '\n' )
#if defined(_MBCS)
			{
				if ( _ismbblead( **strp ) && *(*strp+1) )
					advance = 2;
				else
					advance = 1;
			}
#else
			advance = 1;
#endif
			break;
		case M_EOL:
			if ( **strp == '\n' || **strp == '\0' )
				advance = 0;
			break;
		case M_CCL:
			if ( TSTBIT( read_char( *strp, FFlags ), pat + 1 ) )
				advance = 1;
			break;
		default :						/* literal match */
#if defined(_MBCS)
			if ( *pat == M_NOTASCII )
			{
				unsigned short	c1, c2;

				c1 = *++pat + 0x7F;
				pat++;
				if ( _ismbblead( c1 ) && *pat && *pat <= M_NOTASCII )
				{
					if ( *pat == M_NOTASCII )
						c2 = *++pat + 0x7F;
					else
						c2 = *pat;
					c1 = c1 | (c2 << 8);
					if ( *(unsigned short *)*strp == c1 )
						advance = 2;
				}
				else
					if ( *(unsigned char *)*strp == (pattern)c1 )
						advance = 1;
			}
			else
#endif
				if ( read_char( *strp, FFlags ) == *pat )
					advance = 1;
			break;
	}
	if ( advance > 0 )
		*strp += advance;
	return ( advance + 1 );
}

#if defined(_MBCS)
static const pattern *skipapat( const pattern **pat )
{
	if ( **pat == M_NOTASCII )
	{
		unsigned short	c = *++(*pat) + 0x7F;

		(*pat)++;
		if ( _ismbblead(c) && **pat && **pat <= M_NOTASCII )
		{
			if ( **pat == M_NOTASCII )
				*pat += 2;
			else
				(*pat)++;
		}
	}
	else if ( **pat == (pattern)M_CCL )
		*pat += (MAPSIZE+1);
	else
		(*pat)++;
	return *pat;
}
#endif

#define ISHEXDIGIT(x)		(isdigit(x) || ('a'<=(x) && (x)<='f') || ('A'<=(x) && (x)<='F'))
#define ISOCTDIGIT(x)		('0'<=(x) && (x)<='7')

/* Convert the hex digit represented by 'c' to an int. 'c'
 * must be one of: 0123456789abcdefABCDEF
 */
static int hex2bin( int c )
{
	return ( isdigit(c) ? (c)-'0' : ((toupper(c))-'A')+10) & 0xf;
}

/* Convert the hex digit represented by 'c' to an int. 'c'
 * must be a digit in the range '0'-'7'.
 */
static int oct2bin( int c )
{
	return ( ((c)-'0') & 0x7 );
}

/*------------------------------------------------------------*/

/* Map escape sequences into their equivalent symbols. Return
 * the equivalent ASCII character. *s is advanced past the
 * escape sequence. If no escape sequence is present, the
 * current character is returned and the string is advanced by
 * one. The following are recognized:
 *
 *  \b     backspace
 *  \f     formfeed
 *  \n     newline
 *  \r     carriage return
 *  \s     space
 *  \t     tab
 *  \e     ASCII ESC character ('\033')
 *  \DDD   number formed of 1-3 octal digits
 *  \xDDD  number formed of 1-3 hex digits
 *  \^C    C = any letter. Control code
 */

ccRegexp::pattern __fastcall ccRegexp::esc( const char **s )
{
	int		rval;

	if ( **s != '\\' )
		rval = *((*s)++);
	else
	{
		++(*s);                                 /* Skip the \ */
		switch ( toupper( **s ) )
		{
			case '\0':  rval = '\\';             break;
			case 'B':   rval = '\b' ;            break;
			case 'F':   rval = '\f' ;            break;
			case 'N':   rval = '\n' ;            break;
			case 'R':   rval = '\r' ;            break;
			case 'S':   rval = ' '  ;            break;
			case 'T':   rval = '\t' ;            break;
			case 'E':   rval = '\033';           break;

			case '^':
				rval = *++(*s) ;
				rval = toupper(rval) - '@' ;
				break;
			case 'X':
				rval = 0;
				++(*s);
				if ( ISHEXDIGIT(**s) )
				{
				   rval  = hex2bin( *(*s)++ );
				}
				if ( ISHEXDIGIT(**s) )
				{
				   rval <<= 4;
				   rval  |= hex2bin( *(*s)++ );
				}
				if ( ISHEXDIGIT(**s) )
				{
				   rval <<= 4;
				   rval  |= hex2bin( *(*s)++ );
				}
				--(*s);
				break;
			default:
				if ( !ISOCTDIGIT(**s) )
					rval = **s;
				else
				{
					++(*s);
					rval = oct2bin( *(*s)++ );
					if ( ISOCTDIGIT(**s) )
					{
						rval <<= 3;
						rval  |= oct2bin( *(*s)++ );
					}
					if ( ISOCTDIGIT(**s) )
					{
						rval <<= 3;
						rval  |= oct2bin( *(*s)++ );
					}
					--(*s);
				}
				break;
		}
		++(*s);
	}
	return ( (pattern)rval );
}

};  // namespace cc

