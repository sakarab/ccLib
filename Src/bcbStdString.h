#ifndef BCB_STD_STRING
#define BCB_STD_STRING

#include <System.hpp>
#include <dir.h>

char ReturnChar();

template <class T> struct BcbPCharTrait     {};
template <> struct BcbPCharTrait<char *>    { typedef char  type; };
template <> struct BcbPCharTrait<wchar_t *> { typedef wchar_t  type; };

template <class T> struct BcbCharTrait  {};

template <> struct BcbCharTrait<char>
{
    typedef     char        char_type;

    static char *in_strcpy( char *dest, const char *src )           { return strcpy( dest, src ); }
    static size_t in_strlen( const char *s )                        { return strlen( s ); }
    static const char *in_strrchr( const char *s, int c )           { return strrchr( s, c ); }
    static char *in_strrchr( char *s, int c )                       { return strrchr( s, c ); }
    static int in_strcmp( const char *s1, const char *s2 ) 	        { return strcmp( s1, s2 ); }
    static int in_stricmp( const char *s1, const char *s2 )         { return stricmp( s1, s2 ); }

    static int in_strncmp( const char *s1, const char *s2, size_t maxlen )
                         { return strncmp( s1, s2, maxlen ); }
    static int in_strnicmp( const char *s1, const char *s2, size_t maxlen )
                          { return strnicmp( s1, s2, maxlen ); }
    static int in_fnsplit( const char *path, char *drive, char *dir, char *name, char *ext )
                         { return fnsplit( path, drive, dir, name, ext ); }
};

template <> struct BcbCharTrait<wchar_t>
{
    typedef     wchar_t     char_type;

    static wchar_t *in_strcpy( wchar_t *dest, const wchar_t *src )  { return wcscpy( dest, src ); }
    static size_t in_strlen( const wchar_t *s )                     { return wcslen( s ); }
    static wchar_t *in_strrchr( const wchar_t *s, int c )           { return wcsrchr( s, c ); }
    static int in_strcmp( const wchar_t *s1, const wchar_t *s2 )    { return wcscmp( s1, s2 ); }
    static int in_stricmp( const wchar_t *s1, const wchar_t *s2 )   { return _wcsicmp( s1, s2 ); }

    static int in_strncmp( const wchar_t *s1, const wchar_t *s2, size_t maxlen )
                         { return wcsncmp( s1, s2, maxlen ); }
    static int in_strnicmp( const wchar_t *s1, const wchar_t *s2, size_t maxlen )
                          { return _wcsnicmp( s1, s2, maxlen ); }
    static int in_fnsplit( const wchar_t *path, wchar_t *drive, wchar_t *dir, wchar_t *name, wchar_t *ext )
                         { return _wfnsplit( path, drive, dir, name, ext ); }
};

typedef BcbCharTrait<BcbPCharTrait<PChar>::type>      stru;

#endif
