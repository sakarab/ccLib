#pragma once

#include <Windows.h>
#include <string>

/************************************************************
********    TRegistry
************************************************************/
class TRegistry
{
public:
    enum TRegDataType { rdUnknown, rdString, rdExpandString, rdInteger, rdBinary };
    struct TRegDataInfo
    {
        DWORD           DataSize;
        TRegDataType    RegData;
    };
private:
    HKEY            FRootKey;
    HKEY            FCurrentKey;
    std::wstring    FCurrentPath;
    unsigned int    FAccess;
    bool            FLazyWrite;
    bool            FCloseRootKey;

    HKEY GetBaseKey( bool relative );
    void ChangeKey( HKEY value, const std::wstring& path );
    int GetDataSize( const std::wstring& value_name );
    bool GetDataInfo( const std::wstring& value_name, TRegDataInfo& value );
    int GetData( const std::wstring& name, BYTE *buffer, DWORD buf_size, TRegDataType& reg_data );
    void PutData( const std::wstring& name, const BYTE *buffer, DWORD buf_size, TRegDataType reg_data );
    // noncopyable
    TRegistry( const TRegistry& src );
    TRegistry& operator = ( const TRegistry& src );
public:
    TRegistry();
    TRegistry( DWORD access_rights );
    ~TRegistry();
    void RootKey( HKEY key );
    bool OpenKey( const std::wstring& key, bool can_create );
    bool OpenKey( const TCHAR *key, bool can_create );
    void CloseKey();
    bool ValueExists( const std::wstring& key );
    bool ValueExists( const TCHAR *key );
    bool ReadBool( const std::wstring& key );
    int  ReadInteger( const std::wstring& key );
    std::wstring ReadString( const std::wstring& key );
    void WriteString( const std::wstring& name, const std::wstring& value );
    std::string RegGetValueA( const char *key );
};
