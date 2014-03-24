//---------------------------------------------------------------------------
#ifndef CSyncH
#define CSyncH
//---------------------------------------------------------------------------
#include <windows.h>
#include "ssport.h"

namespace ww
{

class CCriticalSection
{
private:
	CRITICAL_SECTION	FCriticalSection;
	NO_COPY_CTOR(CCriticalSection);
	NO_COPY_OPER(CCriticalSection);
public:
	FASTCALL CCriticalSection();
	FASTCALL ~CCriticalSection();
	void FASTCALL Enter()				{ EnterCriticalSection( &FCriticalSection ); }
	void FASTCALL Leave()				{ LeaveCriticalSection( &FCriticalSection ); }
};

class Locker
{
private:
    CCriticalSection&   FCriticalSection;
	NO_COPY_CTOR(Locker);
	NO_COPY_OPER(Locker);
public:
    FASTCALL Locker( CCriticalSection& critical_section )
        : FCriticalSection(critical_section)
    {
        FCriticalSection.Enter();
    }
    FASTCALL ~Locker()
    {
        FCriticalSection.Leave();
    }
};

}
//---------------------------------------------------------------------------
#endif
