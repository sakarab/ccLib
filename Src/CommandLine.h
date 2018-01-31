//---------------------------------------------------------------------------
#ifndef CommandLineH
#define CommandLineH
//---------------------------------------------------------------------------

//#include <Classes.hpp>
#include <vector>
#include "ccStdString.h"

namespace cc
{

class TCommandLine
{
private:
	std::vector<stru::char_type *>      FArgs;

	void __fastcall ClearArgs();
	int __fastcall GetCount();
	stru::char_type **__fastcall GetArgv();
protected:
public:
	__fastcall TCommandLine( bool auto_parse = false );
	__fastcall ~TCommandLine();
	void __fastcall ParseCommandLine( bool clear_old );
	TCommandLine& __fastcall operator <<( stru::char_type *param );
	__property int argc = { read=GetCount };
	__property stru::char_type **argv = { read=GetArgv };
};

}; // end namespace cc

#endif
 