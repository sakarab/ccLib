//---------------------------------------------------------------------------
#ifndef CIniFilesH
#define CIniFilesH
//---------------------------------------------------------------------------
#include "ssport.h"
#include <System.hpp>
#include <vector>

namespace ww
{

class CCustomIniFile
{
private:
	String		FName;
public:
	FASTCALL CCustomIniFile();
	FASTCALL CCustomIniFile( const CCustomIniFile& src );
	FASTCALL CCustomIniFile( const String& fname );

	CCustomIniFile& operator=( const CCustomIniFile& src );

	virtual String FASTCALL ReadString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Default ) = 0;
	virtual void FASTCALL WriteString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Value ) = 0;

	bool FASTCALL ReadBool( const wchar_t *Section, const wchar_t *Ident, bool Default );
	long FASTCALL ReadInteger( const wchar_t *Section, const wchar_t *Ident, long Default );
	TDateTime FASTCALL ReadDateTime( const wchar_t *Section, const wchar_t *Ident, const TDateTime& Default );
	double FASTCALL ReadFloat( const wchar_t *Section, const wchar_t *Ident, const double& Default );

    void FASTCALL ReadSection( const wchar_t *Section, std::vector<String>& strings );

	void FASTCALL WriteBool( const wchar_t *Section, const wchar_t *Ident, bool Value );
	void FASTCALL WriteInteger( const wchar_t *Section, const wchar_t *Ident, long Value);
	void FASTCALL WriteDateTime( const wchar_t *Section, const wchar_t *Ident, const TDateTime& Value );
	void FASTCALL WriteFloat( const wchar_t *Section, const wchar_t *Ident, const double& Value );

	String FASTCALL GetFileName()                           { return ( FName ); }
};

class CIniFile : public CCustomIniFile
{
private:
protected:
public:
	FASTCALL CIniFile();
	FASTCALL CIniFile( String fname );
	virtual String FASTCALL ReadString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Default );
	virtual void FASTCALL WriteString( const wchar_t *Section, const wchar_t *Ident, const wchar_t *Value );
};

}
//---------------------------------------------------------------------------
#endif
