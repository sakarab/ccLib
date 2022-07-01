//---------------------------------------------------------------------------
#ifndef CIniFilesH
#define CIniFilesH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <vector>
#include "ccStdString.h"

namespace ww
{

class CCustomIniFile
{
private:
	String		FName;
public:
	CCustomIniFile();
	CCustomIniFile( const CCustomIniFile& src );
	CCustomIniFile( const String& fname );

	CCustomIniFile& operator=( const CCustomIniFile& src );

	virtual String ReadString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Default ) = 0;
	virtual void WriteString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Value ) = 0;

	bool ReadBool( const stru::char_type *Section, const stru::char_type *Ident, bool Default );
	long ReadInteger( const stru::char_type *Section, const stru::char_type *Ident, long Default );
	TDateTime ReadDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Default );
	double ReadFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Default );

    void ReadSection( const stru::char_type *Section, std::vector<String>& strings );

	void WriteBool( const stru::char_type *Section, const stru::char_type *Ident, bool Value );
	void WriteInteger( const stru::char_type *Section, const stru::char_type *Ident, long Value);
	void WriteDateTime( const stru::char_type *Section, const stru::char_type *Ident, const TDateTime& Value );
	void WriteFloat( const stru::char_type *Section, const stru::char_type *Ident, const double& Value );

	String GetFileName()                           { return ( FName ); }
};

class CIniFile : public CCustomIniFile
{
private:
protected:
public:
	CIniFile();
	CIniFile( String fname );
	virtual String ReadString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Default );
	virtual void WriteString( const stru::char_type *Section, const stru::char_type *Ident, const stru::char_type *Value );
};

}
//---------------------------------------------------------------------------
#endif
