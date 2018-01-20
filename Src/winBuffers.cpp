//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <UtilCls.h>
#include "Buffers.h"
#include <vector>

//---------------------------------------------------------------------------
namespace vcl
{

class Buffer
{
private:
	TAPtr<char>		buff;
	int				len;
	void FASTCALL AllocBuffer( int new_len )
	{
		if ( buff != NULL && len >= new_len )
			return;
		buff = new char[new_len+1];
		len = new_len;
	}
    NO_COPY_CTOR(Buffer);
    NO_COPY_OPER(Buffer);
public:
	FASTCALL Buffer()
        : buff(0), len(0)
	{
	}
	void FASTCALL CopyStr( char *str, int len );
	char * FASTCALL GetBuffer()
	{
		return ( buff );
	}
};

void FASTCALL Buffer::CopyStr( char *str, int len )
{
	AllocBuffer( len );
	strncpy( buff, str, len );
	buff[len] = '\0';
}

void FASTCALL TStringsStreamBuf::AppendToControl()
{
	char		*ptr, *buffer = pbase(), *end_buffer = pptr();
	bool		old_LastWasCR;
	Buffer		Buff;

	old_LastWasCR = LastWasCR;
	try
	{
		vcl_ptr->BeginUpdate();
		try
		{
			while ( buffer < end_buffer )
			{
				if ( LastChar == '\r' )
					if ( buffer[0] == '\n' )
						buffer++;
				ptr = buffer;
				while ( ptr < end_buffer )
				{
					if ( *ptr == '\r' || *ptr == '\n' || *ptr == '\0' )
						break;
					ptr++;
				}
				Buff.CopyStr( buffer, ptr-buffer );
				if ( ! LastWasCR )
				{
					int		str_count = vcl_ptr->Count - 1;
					vcl_ptr->Strings[str_count] = vcl_ptr->Strings[str_count] + AnsiString( Buff.GetBuffer() );
				}
				else
					vcl_ptr->Add( AnsiString( Buff.GetBuffer() ) );
				LastChar = *ptr;
				LastWasCR = ( *ptr == '\r' || *ptr == '\n' || *ptr == '\0' );
				buffer = ptr + 1;
			}
		}
		__finally
		{
			vcl_ptr->EndUpdate();
		}
	}
	catch( ... )
	{
		LastWasCR = old_LastWasCR;
		throw;
	}
}

void FASTCALL TStringsStreamBuf::WriteChar( char ch )
{
	bool		old_LastWasCR;

	old_LastWasCR = LastWasCR;
	try
	{
		if ( LastChar == '\r' && ch == '\n' )
			return;
		if ( ch != '\n' )
		{
			if ( ! LastWasCR )
				vcl_ptr->Strings[vcl_ptr->Count - 1] = vcl_ptr->Strings[vcl_ptr->Count - 1] + ch;
			else
				vcl_ptr->Add( ch );
			LastChar = ch;
		}
		LastWasCR = ( ch == '\r' || ch == '\n' || ch == '\0' );
	}
	catch( ... )
	{
		LastWasCR = old_LastWasCR;
		throw;
	}
}

//--------------------------------------------------------------------------------------
void FASTCALL TMemoStreambuf::InternalAppendToControl( const char * const str )
{
    int     len = SendMessage( WinHandle, WM_GETTEXTLENGTH, 0, 0 );

    SendMessage( WinHandle, EM_SETSEL, len, len );
    SendMessage( WinHandle, EM_REPLACESEL, 0, reinterpret_cast<LONG>(str) );
}

void FASTCALL TMemoStreambuf::AppendToControl()
{
	char    *start = pbase(), *end = pptr();

    if ( start < end )
    {
        std::vector<char>   buff;
        char                ch;

        while ( start < end )
        {
            ch = *start++;
            if ( ch == '\n' || ch == '\0' )
            {
                buff.push_back( '\r' );
                buff.push_back( '\n' );
            }
            else if ( ch != '\r' )
                buff.push_back( ch );
        }
        buff.push_back( '\0' );
        InternalAppendToControl( &buff[0] );
    }
}

void FASTCALL TMemoStreambuf::WriteChar( char ch )
{
    char    str[2];

    str[0] = ch;
    str[1] = 0;
    InternalAppendToControl( str );
}
//--------------------------------------------------------------------------------------
void FASTCALL TRitchEditStreambuf::InternalAppendToControl( const char * const str )
{
    int     len = SendMessage( WinHandle, WM_GETTEXTLENGTH, 0, 0 );

    SendMessage( WinHandle, EM_SETSEL, len, len );
    SendMessage( WinHandle, EM_REPLACESEL, 0, reinterpret_cast<LONG>(str) );
}

void FASTCALL TRitchEditStreambuf::AppendToControl()
{
	char    *start = pbase(), *end = pptr();

    if ( start < end )
    {
        TAPtr<char>     buff( new char[(end-start)+1] );
        char            ch, *dst = buff;

        while ( start < end )
            *dst++ = (ch = *start++) != 0 ? ch : '\n';
        *dst = 0;
        InternalAppendToControl( buff );
    }
}

void FASTCALL TRitchEditStreambuf::WriteChar( char ch )
{
    char    str[2];

    str[0] = ch != 0 ? ch : '\n';
    str[1] = 0;
    InternalAppendToControl( str );
}

}; // namespace vcl
//--------------------------------------------------------------------------------------

