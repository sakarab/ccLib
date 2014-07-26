#pragma once

#if ! defined (CC_BOM_UTILS_H)
#define CC_BOM_UTILS_H

#include <string>
#if defined(QT_CORE_LIB)
#include <QString>
#endif

/**************************************************************************
********    BOM
**************************************************************************/
struct BOM
{
private:
    // noncopyable
    BOM( const BOM& src );
    BOM& operator=( const BOM& src );
    // nonconstructible
    BOM();
private:
    static wchar_t const *Names[];
public:
    enum type { no_bom, utf8, utf16le, utf16be, utf32le, utf32be };

    static std::wstring Name( type bom );
    static type Value( std::wstring name );
};

/**************************************************************************
********    Free functions
**************************************************************************/
#if defined(QT_CORE_LIB)
BOM::type GetBom( const QString& file_name );
#endif

BOM::type GetBom( const std::string& file_name );
BOM::type GetBom( const char *file_name );

#if defined (WIN32)
BOM::type GetBom( const std::wstring& file_name );
BOM::type GetBom( const wchar_t *file_name );
#endif

#endif
