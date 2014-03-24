//---------------------------------------------------------------------------
#ifndef ccUtilsH
#define ccUtilsH
//---------------------------------------------------------------------------

#include <System.hpp>
#include "ssport.h"

extern	const char	    EmptyChar[];
extern	const wchar_t	EmptyCharW[];

String BaseName( const String& sstr );
String AddSlash( const String& fname );
void AddSlash( wchar_t *fname );
void ThrowException( wchar_t *mess );
void ThrowExceptionFmt( wchar_t *fmt, ... );

String GetWinVersionInfo();
String GetDllVersion( const String& fname );

namespace vcl
{

#if ( __BORLANDC__ >= 0x0560 )          // C++ Bulder 6

template <class INTEF> inline INTEF DynamicCast( TObject *obj )
{
	INTEF	result = 0;

	if ( obj != NULL )
	{
		TInterfaceEntry		*intef_entry;

		if ( (intef_entry = TObject::GetInterfaceEntry( obj->ClassType(), __uuidof(INTEF) )) != 0 )
			if ( intef_entry->IOffset != 0 )
				result = (INTEF)((int)obj + intef_entry->IOffset);
	}
	return ( result );
}

#endif

template <class OBJ> void FreeAndNil( OBJ*& obj )
{
    delete obj;
    obj = NULL;
}

template <class T> class UpdateLocker
{
private:
    T   *FObj;
    NO_COPY_CTOR(UpdateLocker);
    NO_COPY_OPER(UpdateLocker);
public:
    FASTCALL UpdateLocker( T*& obj ) : FObj(obj)            { FObj->BeginUpdate(); }
    FASTCALL UpdateLocker( T* const & obj ) : FObj(obj)     { FObj->BeginUpdate(); }
    FASTCALL ~UpdateLocker()                                { FObj->EndUpdate(); }
};

class HandleCloser
{
private:
    const HANDLE&   Hand;
    NO_COPY_CTOR(HandleCloser);
    NO_COPY_OPER(HandleCloser);
public:
    FASTCALL HandleCloser( const HANDLE& hnd ) : Hand(hnd)      { /* empty */ }
    FASTCALL ~HandleCloser()                                    { CloseHandle( Hand ); }
};

};  // end namespace vcl

//---------------------------------------------------------------------------
#endif
