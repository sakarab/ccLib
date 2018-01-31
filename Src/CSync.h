//---------------------------------------------------------------------------
#ifndef CSyncH
#define CSyncH
//---------------------------------------------------------------------------
#include <windows.h>

namespace ww
{

class CCriticalSection
{
private:
	CRITICAL_SECTION	FCriticalSection;
	NO_COPY_CTOR(CCriticalSection);
	NO_COPY_OPER(CCriticalSection);
public:
	CCriticalSection();
	~CCriticalSection();
	void Enter()				{ EnterCriticalSection( &FCriticalSection ); }
	void Leave()				{ LeaveCriticalSection( &FCriticalSection ); }
};

class Locker
{
private:
    CCriticalSection&   FCriticalSection;
	NO_COPY_CTOR(Locker);
	NO_COPY_OPER(Locker);
public:
    Locker( CCriticalSection& critical_section )
        : FCriticalSection(critical_section)
    {
        FCriticalSection.Enter();
    }
    ~Locker()
    {
        FCriticalSection.Leave();
    }
};

}
//---------------------------------------------------------------------------
#endif
