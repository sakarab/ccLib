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
#include <pre_cc.h>
#include "winBuffers.h"
#include "cpp_string.h"
#include <vector>
#include <boost/smart_ptr.hpp>

namespace ccwin
{
    //=====================================================================
    //==============    NewStreamBuf
    //=====================================================================
    NewStreamBuf::NewStreamBuf( unsigned int size )
        : std::wstreambuf()
    {
        char_type   *ptr;

        if ( size > 0 )
        {
            ptr = new char_type[size];
            setp( ptr, ptr + size );		// set output poiners
        }
        else
            setp( 0, 0 );					// set output poiners
        setg( 0, 0, 0 );					// set input poiners
    }

    NewStreamBuf::~NewStreamBuf()
    {
        if ( pbase() != nullptr )
            delete[] pbase();
    }

    NewStreamBuf::inherited::int_type NewStreamBuf::overflow( int_type ch )
    {
        if ( pbase() == epptr() )
        {
            if ( ch != std::char_traits<inherited::char_type>::eof() )
                WriteChar( static_cast<char_type>(ch) );        // unbuffered streambuf
        }
        else
        {
            WriteBuffer();
            if ( ch != std::char_traits<inherited::char_type>::eof() )
                sputc( static_cast<char_type>(ch) );            // buffered streambuf
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
        char_type   *buffer = pbase();

        if ( pptr() - buffer > 0 )
        {
            AppendToControl();
            setp( buffer, epptr() );
        }
    }

//=====================================================================
//==============    Buffer
//=====================================================================
//class Buffer
//{
//private:
//    TAPtr<char>		buff;
//    int				len;
//    void AllocBuffer( int new_len )
//    {
//        if ( buff != NULL && len >= new_len )
//            return;
//        buff = new char[new_len + 1];
//        len = new_len;
//    }
//    Buffer( const Buffer& ) CC_EQ_DELETE;
//    Buffer& operator = ( const Buffer& ) CC_EQ_DELETE;
//public:
//    Buffer()
//        : buff( 0 ), len( 0 )
//    {}
//    void CopyStr( char *str, int len );
//    char * GetBuffer() { return (buff); }
//};
//
//void Buffer::CopyStr( char *str, int len )
//{
//    AllocBuffer( len );
//    strncpy( buff, str, len );
//    buff[len] = '\0';
//}

//=====================================================================
//==============    TStringsStreamBuf
//=====================================================================
//void FASTCALL TStringsStreamBuf::AppendToControl()
//{
//	char		*ptr, *buffer = pbase(), *end_buffer = pptr();
//	bool		old_LastWasCR;
//	Buffer		Buff;
//
//	old_LastWasCR = LastWasCR;
//	try
//	{
//		vcl_ptr->BeginUpdate();
//		try
//		{
//			while ( buffer < end_buffer )
//			{
//				if ( LastChar == '\r' )
//					if ( buffer[0] == '\n' )
//						buffer++;
//				ptr = buffer;
//				while ( ptr < end_buffer )
//				{
//					if ( *ptr == '\r' || *ptr == '\n' || *ptr == '\0' )
//						break;
//					ptr++;
//				}
//				Buff.CopyStr( buffer, ptr-buffer );
//				if ( ! LastWasCR )
//				{
//					int		str_count = vcl_ptr->Count - 1;
//					vcl_ptr->Strings[str_count] = vcl_ptr->Strings[str_count] + AnsiString( Buff.GetBuffer() );
//				}
//				else
//					vcl_ptr->Add( AnsiString( Buff.GetBuffer() ) );
//				LastChar = *ptr;
//				LastWasCR = ( *ptr == '\r' || *ptr == '\n' || *ptr == '\0' );
//				buffer = ptr + 1;
//			}
//		}
//		__finally
//		{
//			vcl_ptr->EndUpdate();
//		}
//	}
//	catch( ... )
//	{
//		LastWasCR = old_LastWasCR;
//		throw;
//	}
//}
//
//void FASTCALL TStringsStreamBuf::WriteChar( char ch )
//{
//	bool		old_LastWasCR;
//
//	old_LastWasCR = LastWasCR;
//	try
//	{
//		if ( LastChar == '\r' && ch == '\n' )
//			return;
//		if ( ch != '\n' )
//		{
//			if ( ! LastWasCR )
//				vcl_ptr->Strings[vcl_ptr->Count - 1] = vcl_ptr->Strings[vcl_ptr->Count - 1] + ch;
//			else
//				vcl_ptr->Add( ch );
//			LastChar = ch;
//		}
//		LastWasCR = ( ch == '\r' || ch == '\n' || ch == '\0' );
//	}
//	catch( ... )
//	{
//		LastWasCR = old_LastWasCR;
//		throw;
//	}
//}

//=====================================================================
//==============    TMemoStreambuf
//=====================================================================
void TMemoStreambuf::InternalAppendToControl( const char_type * const str )
{
    int     len = SendMessage( mWinHandle, WM_GETTEXTLENGTH, 0, 0 );

    SendMessage( mWinHandle, EM_SETSEL, len, len );
    SendMessage( mWinHandle, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(const_cast<char_type *>(str)) );
}

void TMemoStreambuf::AppendToControl()
{
    char_type   *start = pbase(), *end = pptr();

    if ( start < end )
    {
        std::vector<char_type>      buff;
        char_type                   ch;

        while ( start < end )
        {
            ch = *start++;
            if ( ch == cclib::CharConstant<char_type>::lf || ch == '\0' )
            {
                buff.push_back( cclib::CharConstant<char_type>::cr );
                buff.push_back( cclib::CharConstant<char_type>::lf );
            }
            else if ( ch != cclib::CharConstant<char_type>::cr )
                buff.push_back( ch );
        }
        buff.push_back( '\0' );
        InternalAppendToControl( &buff[0] );
    }
}

void TMemoStreambuf::WriteChar( char_type ch )
{
    char_type   str[2];

    str[0] = ch;
    str[1] = 0;
    InternalAppendToControl( str );
}

//=====================================================================
//==============    TRitchEditStreambuf
//=====================================================================
void TRitchEditStreambuf::InternalAppendToControl( const char_type * const str )
{
    int     len = SendMessage( mWinHandle, WM_GETTEXTLENGTH, 0, 0 );

    SendMessage( mWinHandle, EM_SETSEL, len, len );
    SendMessage( mWinHandle, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(const_cast<char_type *>(str)) );
}

void TRitchEditStreambuf::AppendToControl()
{
    char_type   *start = pbase(), *end = pptr();

    if ( start < end )
    {
        boost::scoped_array<char_type>      buff( new char_type[(end-start)+1] );
        char_type                           ch, *dst = buff.get();

        while ( start < end )
            *dst++ = (ch = *start++) != 0 ? ch : cclib::CharConstant<char_type>::lf;
        *dst = 0;
        InternalAppendToControl( buff.get() );
    }
}

void TRitchEditStreambuf::WriteChar( char_type ch )
{
    char_type   str[2];

    str[0] = ch != 0 ? ch : cclib::CharConstant<char_type>::lf;
    str[1] = 0;
    InternalAppendToControl( str );
}

}; // namespace ccwin
