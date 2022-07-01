//---------------------------------------------------------------------------
#ifndef FRefCntH
#define FRefCntH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <string>
//---------------------------------------------------------------------------
class TfrmRefCnt : public TForm
{
__published:	// IDE-managed Components
    TButton *Button1;
    TButton *Button2;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRefCnt(TComponent* Owner);
};
//---------------------------------------------------------------------------

class rcData
{
private:
    bool        Defined;
    int         Value;
    std::string Name;
    rcData( const rcData& src );
    rcData& operator=( const rcData& src );
public:
    rcData();
    rcData( const std::string& name );
    rcData( int value, const std::string& name );
    ~rcData();
};

//---------------------------------------------------------------------------
#endif
