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

    //=====================================================================
    //==============    NewStreamBuf
    //=====================================================================
    class NewStreamBuf : public std::wstreambuf
    {
    private:
        typedef std::wstreambuf     inherited;
    private:
        void WriteBuffer();
    protected:
        virtual void AppendToControl() = 0;
        virtual void WriteChar( char_type ch ) = 0;
        //  from streambuf - these have to send the text to the memo
        virtual int_type overflow( int_type ch ) CC_OVERRIDE;
        virtual int sync() CC_OVERRIDE;
    public:
        NewStreamBuf( unsigned int size = 2048 );
        virtual ~NewStreamBuf() CC_OVERRIDE;
    };

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

        void InternalAppendToControl( const char_type * const str );
    protected:
        virtual void AppendToControl() CC_OVERRIDE;
        virtual void WriteChar( char_type ch ) CC_OVERRIDE;
    public:
        TMemoStreambuf( HWND memo, unsigned int size = 2048 )
            : NewStreamBuf( size ), mWinHandle( memo )
        {}
    };

    //=====================================================================
    //==============    TRitchEditStreambuf
    //=====================================================================
    class TRitchEditStreambuf : public NewStreamBuf
    {
    private:
        HWND    mWinHandle;

        void InternalAppendToControl( const char_type * const str );
    protected:
        virtual void AppendToControl() CC_OVERRIDE;
        virtual void WriteChar( char_type ch ) CC_OVERRIDE;
    public:
        TRitchEditStreambuf() : NewStreamBuf( 2048 ), mWinHandle()                                      {}
        explicit TRitchEditStreambuf( unsigned int size ) : NewStreamBuf( size ), mWinHandle()          {}

        void Attach( HWND win_handle )                                                                  { mWinHandle = win_handle; }
    };

} // namespace ccwin

#endif
