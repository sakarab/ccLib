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
		stru::char_type     fname[500];

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
    for ( std::vector<stru::char_type *>::iterator n = FArgs.begin() ; n != FArgs.end() ; ++n )
        delete [] *n;
}

void __fastcall TCommandLine::ParseCommandLine( bool clear_old )
{
	stru::char_type		str[500];
	stru::char_type		*p1, *p2;
	int			        pass_count;

	if ( clear_old )
		ClearArgs();
	stru::in_strcpy( str, GetCommandLine() );
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

TCommandLine& __fastcall TCommandLine::operator <<( stru::char_type *param )
{
	stru::char_type     str[500], *tmpstr;
	int                 len;

	if ( *param == QUOTE )
		param++;
	stru::in_strcpy( str, param );
	len = stru::in_strlen( str );
	if ( len > 0 && str[len-1] == QUOTE )
		str[len-1] = EOS;
	tmpstr = new stru::char_type[len];
	try
	{
		FArgs.push_back( tmpstr );
		stru::in_strcpy( tmpstr, str );
	}
	catch(...)
	{
		delete[] tmpstr;
	}
	return ( *this );
}

stru::char_type **__fastcall TCommandLine::GetArgv()
{
    return ( &FArgs.front() );
}

}; // end namespace cc

