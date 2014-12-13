//---------------------------------------------------------------------------
#ifndef CIniFilesH
#define CIniFilesH
//---------------------------------------------------------------------------
#include "ssport.h"
#include <System.hpp>
#include <vector>
#include "bcbStdString.h"

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

	virtual String FASTCALL ReadString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Default ) = 0;
	virtual void FASTCALL WriteString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Value ) = 0;

	bool FASTCALL ReadBool( const stru::char_type *Section, const stru::char_type *Ident, bool Default );
	long FASTCALL ReadInteger( const stru::char_type *Section, const stru::char_type *Ident, long Default );
	TDateTime FASTCALL ReadDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Default );
	double FASTCALL ReadFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Default );

    void FASTCALL ReadSection( const stru::char_type *Section, std::vector<String>& strings );

	void FASTCALL WriteBool( const stru::char_type *Section, const stru::char_type *Ident, bool Value );
	void FASTCALL WriteInteger( const stru::char_type *Section, const stru::char_type *Ident, long Value);
	void FASTCALL WriteDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Value );
	void FASTCALL WriteFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Value );

	String FASTCALL GetFileName()                           { return ( FName ); }
};

class CIniFile : public CCustomIniFile
{
private:
protected:
public:
	FASTCALL CIniFile();
	FASTCALL CIniFile( String fname );
	virtual String FASTCALL ReadString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Default );
	virtual void FASTCALL WriteString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Value );
};

}
//---------------------------------------------------------------------------
#endif
