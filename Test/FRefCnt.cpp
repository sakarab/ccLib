//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FRefCnt.h"
#include "RefCnt.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
rcData::rcData()
    : Defined(false), Name("")
{
}

rcData::rcData( const std::string& name )
    : Defined(false), Name(name)
{
}

rcData::rcData( int value, const std::string& name )
    : Defined(true), Value(value), Name(name)
{
}

rcData::~rcData()
{
}
//---------------------------------------------------------------------------
__fastcall TfrmRefCnt::TfrmRefCnt(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRefCnt::Button1Click(TObject *Sender)
{
    cclib::Handle<rcData>       aa;
    cclib::Handle<rcData>       bb( 5, "aaaa" );
    cclib::Handle<rcData>       cc( bb );

    std::string                 str( "bbbb" );

    cclib::Handle<rcData>       dd( str );
}
//---------------------------------------------------------------------------
unsigned __int64 foot()
{
    __int64   aa = 5, bb = 6, cc;

    cc = aa + bb + sizeof(__int64);
    return ( cc );
}

void __fastcall TfrmRefCnt::Button2Click(TObject *Sender)
{
    foot();
}
//---------------------------------------------------------------------------

