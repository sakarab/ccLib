//---------------------------------------------------------------------------
#ifndef BuffersH
#define BuffersH
//---------------------------------------------------------------------------
#include <streambuf>
#include "ssport.h"
#include <StdCtrls.hpp>

namespace cc
{

class NewStreamBuf : public std::streambuf
{
private:
	void FASTCALL WriteBuffer();
protected:
	virtual void FASTCALL AppendToControl() = 0;
	virtual void FASTCALL WriteChar( char ch ) = 0;
	//  from streambuf - these have to send the text to the memo
	virtual int overflow( int_type ch );
	virtual int sync();
public:
	FASTCALL NewStreamBuf( unsigned int size = 2048 );
	virtual ~NewStreamBuf();
};

FASTCALL NewStreamBuf::NewStreamBuf( unsigned int size )
	: std::streambuf()
{
	char	*ptr;

	if ( size > 0 )
	{
		ptr = new char[size];
		setp( ptr, ptr + size );		// set output poiners
	}
	else
		setp( 0, 0 );					// set output poiners
	setg( 0, 0, 0 );					// set input poiners
}

NewStreamBuf::~NewStreamBuf()
{
	if ( pbase() != NULL )
		delete[] pbase();
}

int NewStreamBuf::overflow( int_type ch )
{
	if ( pbase() == epptr() )
	{
		if ( ch != EOF )
			WriteChar( (char_type)ch );		// unbuffered streambuf
	}
	else
	{
		WriteBuffer();
		if ( ch != EOF )
			sputc( (char_type)ch );			// buffered streambuf
	}
	return 0;
}

int NewStreamBuf::sync()
{
	WriteBuffer();
	return 0;
}

void FASTCALL NewStreamBuf::WriteBuffer()
{
	char	*buffer = pbase();

	if ( pptr() - buffer > 0 )
	{
		AppendToControl();
		setp( buffer, epptr() );
	}
}

}; // namespace cc

namespace vcl
{

class TStringsStreamBuf : public cc::NewStreamBuf
{
private:
	TStrings    *vcl_ptr;
	bool	    LastWasCR;
	char	    LastChar;
protected:
	virtual void FASTCALL AppendToControl();
	virtual void FASTCALL WriteChar( char ch );
public:
	FASTCALL TStringsStreamBuf( TStrings* strings, unsigned int size = 2048 )
        : cc::NewStreamBuf( size ), vcl_ptr(strings), LastWasCR(true)
	{
	}
};

class TListBoxStreambuf : public TStringsStreamBuf
{
public:
	FASTCALL TListBoxStreambuf( TListBox* list_box, unsigned int size = 2048 )
		: TStringsStreamBuf( list_box->Items, size )
    {
    }
};

class TComboBoxStreambuf : public TStringsStreamBuf
{
public:
	FASTCALL TComboBoxStreambuf( TComboBox* combo_box, unsigned int size = 2048 )
		: TStringsStreamBuf( combo_box->Items, size )
    {
    }
};

class TMemoStreambuf : public cc::NewStreamBuf
{
private:
    HWND    WinHandle;
	void FASTCALL InternalAppendToControl( const char * const str );
protected:
	virtual void FASTCALL AppendToControl();
	virtual void FASTCALL WriteChar( char ch );
public:
	FASTCALL TMemoStreambuf( TWinControl *memo, unsigned int size = 2048 )
		: cc::NewStreamBuf( size ), WinHandle(memo->Handle)
    {
    }
};

class TRitchEditStreambuf : public cc::NewStreamBuf
{
private:
    HWND    WinHandle;
	void FASTCALL InternalAppendToControl( const char * const str );
protected:
	virtual void FASTCALL AppendToControl();
	virtual void FASTCALL WriteChar( char ch );
public:
	FASTCALL TRitchEditStreambuf( TWinControl *rich_edit, unsigned int size = 2048 )
		: cc::NewStreamBuf( size ), WinHandle(rich_edit->Handle)
    {
    }
};

}; // namespace vcl
//---------------------------------------------------------------------------
#endif
