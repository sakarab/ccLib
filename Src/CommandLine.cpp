//---------------------------------------------------------------------------
#pragma hdrstop

#include "CommandLine.h"
#include "windows.h"
//---------------------------------------------------------------------------

#define	EOS		'\0'
#define	QUOTE	'\"'

namespace cc
{

__fastcall TCommandLine::TCommandLine( bool auto_parse )
    : FArgs()
{
	if ( auto_parse == true )
		ParseCommandLine( false );
	else
	{
		wchar_t     fname[500];

		fname[0] = 0;
		GetModuleFileName( NULL, fname, 500 );
		*this << fname;
	}
}

__fastcall TCommandLine::~TCommandLine()
{
    ClearArgs();
}

int __fastcall TCommandLine::GetCount()
{
	return ( FArgs.size() );
}

void __fastcall TCommandLine::ClearArgs()
{
    for ( std::vector<wchar_t *>::iterator n = FArgs.begin() ; n != FArgs.end() ; ++n )
        delete [] *n;
}

void __fastcall TCommandLine::ParseCommandLine( bool clear_old )
{
	wchar_t		str[500];
	wchar_t		*p1, *p2;
	int			pass_count;

	if ( clear_old )
		ClearArgs();
	wcscpy( str, GetCommandLine() );
	p1 = str;
	pass_count = 0;
	while ( *p1 != EOS )
	{
		while ( true ) {
			while ( *p1 != EOS && *p1 <= ' ' )
				p1++;
			if ( *p1 == QUOTE && *(p1 + 1) == QUOTE )
				p1 += 2;
			else
				break;
		}
		p2 = p1;
		while ( *p1 > ' ' )
		{
			if ( *p1 == QUOTE )
			{
				p1++;
				while ( *p1 != EOS && *p1 != QUOTE )
					p1++;
				if ( *p1 != EOS )
					p1++;
			}
			else
				p1++;
		}
		if ( *p1 != EOS )
		{
			p1++;
			*(p1 - 1) = EOS;
		}
		if ( pass_count != 0 || GetCount() < 1 )
			*this << p2;
		pass_count++;
	}
}

TCommandLine& __fastcall TCommandLine::operator <<( wchar_t *param )
{
	wchar_t     str[500], *tmpstr;
	int         len;

	if ( *param == QUOTE )
		param++;
	wcscpy( str, param );
	len = wcslen( str );
	if ( len > 0 && str[len-1] == QUOTE )
		str[len-1] = EOS;
	tmpstr = new wchar_t[len];
	try
	{
		FArgs.push_back( tmpstr );
		wcscpy( tmpstr, str );
	}
	catch(...)
	{
		delete[] tmpstr;
	}
	return ( *this );
}

wchar_t **__fastcall TCommandLine::GetArgv()
{
    return ( &FArgs.front() );
}

}; // end namespace cc

