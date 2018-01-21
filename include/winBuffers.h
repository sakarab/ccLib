//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#ifndef CCLIB_WIN_BUFFERS_H
#define CCLIB_WIN_BUFFERS_H

#include <streambuf>
#include <Windows.h>

namespace ccwin
{

class NewStreamBuf : public std::streambuf
{
private:
	void WriteBuffer();
protected:
	virtual void AppendToControl() = 0;
	virtual void WriteChar( char ch ) = 0;
	//  from streambuf - these have to send the text to the memo
	int overflow( int_type ch );
	int sync();
public:
	NewStreamBuf( unsigned int size = 2048 );
	virtual ~NewStreamBuf();
};

NewStreamBuf::NewStreamBuf( unsigned int size )
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

void NewStreamBuf::WriteBuffer()
{
	char	*buffer = pbase();

	if ( pptr() - buffer > 0 )
	{
		AppendToControl();
		setp( buffer, epptr() );
	}
}

//
//class TStringsStreamBuf : public cc::NewStreamBuf
//{
//private:
//	TStrings    *vcl_ptr;
//	bool	    LastWasCR;
//	char	    LastChar;
//protected:
//	virtual void FASTCALL AppendToControl();
//	virtual void FASTCALL WriteChar( char ch );
//public:
//	FASTCALL TStringsStreamBuf( TStrings* strings, unsigned int size = 2048 )
//        : cc::NewStreamBuf( size ), vcl_ptr(strings), LastWasCR(true)
//	{
//	}
//};
//
//class TListBoxStreambuf : public TStringsStreamBuf
//{
//public:
//	FASTCALL TListBoxStreambuf( TListBox* list_box, unsigned int size = 2048 )
//		: TStringsStreamBuf( list_box->Items, size )
//    {
//    }
//};
//
//class TComboBoxStreambuf : public TStringsStreamBuf
//{
//public:
//	FASTCALL TComboBoxStreambuf( TComboBox* combo_box, unsigned int size = 2048 )
//		: TStringsStreamBuf( combo_box->Items, size )
//    {
//    }
//};

//=====================================================================
//==============    TMemoStreambuf
//=====================================================================
class TMemoStreambuf : public NewStreamBuf
{
private:
    HWND    mWinHandle;

    void InternalAppendToControl( const char * const str );
protected:
	virtual void AppendToControl();
	virtual void WriteChar( char ch );
public:
	TMemoStreambuf( HWND memo, unsigned int size = 2048 )
		: NewStreamBuf( size ), mWinHandle(memo)
    {
    }
};

//=====================================================================
//==============    TRitchEditStreambuf
//=====================================================================
class TRitchEditStreambuf : public NewStreamBuf
{
private:
    HWND    mWinHandle;

    void InternalAppendToControl( const char * const str );
protected:
	virtual void AppendToControl();
	virtual void WriteChar( char ch );
public:
	TRitchEditStreambuf( HWND rich_edit, unsigned int size = 2048 )
		: NewStreamBuf( size ), mWinHandle(rich_edit)
    {
    }
};

} // namespace ccwin

#endif
