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
    // non-copyable
    CCriticalSection( const CCriticalSection& );
    CCriticalSection& operator=( const CCriticalSection& );
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
    // non-copyable
    Locker( const Locker& );
    Locker& operator=( const Locker& );
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
