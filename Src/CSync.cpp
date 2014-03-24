//---------------------------------------------------------------------------
#pragma hdrstop

#include "CSync.h"
//---------------------------------------------------------------------------
namespace ww
{
//---------------------------------------------------------------------------
//-- CCriticalSection
//---------------------------------------------------------------------------
__fastcall CCriticalSection::CCriticalSection()
{
	InitializeCriticalSectionAndSpinCount( &FCriticalSection, 4000 );
}

__fastcall CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection( &FCriticalSection );
}
//---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------

