//---------------------------------------------------------------------------
#ifndef CommandLineH
#define CommandLineH
//---------------------------------------------------------------------------

//#include <Classes.hpp>
#include <vector>

namespace cc
{

class TCommandLine
{
private:
	std::vector<wchar_t *>     FArgs;

	void __fastcall ClearArgs();
	int __fastcall GetCount();
	wchar_t **__fastcall GetArgv();
protected:
public:
	__fastcall TCommandLine( bool auto_parse = false );
	__fastcall ~TCommandLine();
	void __fastcall ParseCommandLine( bool clear_old );
	TCommandLine& __fastcall operator <<( wchar_t *param );
	__property int argc = { read=GetCount };
	__property wchar_t **argv = { read=GetArgv };
};

}; // end namespace cc

#endif
 