//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#ifndef CC_LIB_ODBC_H
#define CC_LIB_ODBC_H

#if defined (WIN32) || defined (__WIN32__) || defined (_WIN32)
    #include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <tuple>
#include <cpp_string.h>
#include <cc_db.h>

namespace ccodbc
{

    struct TypeRaw
    {
        SWORD       OdbcType;
        SWORD       OdbcCType;
        ccdb::type  dbType;
        int         BinarySize;
    };

    //=======================================================================
    //======    Field
    //=======================================================================
    class Field
    {
    private:
        enum { BUFFER_SWITCH = 16 };
        enum { LONG_DATA = 256 };
        enum { LONG_DATA_BUFFER = 1024 };

        UDWORD              mPrecision;
        SWORD               mScale;
        SWORD               mNullable;
        SQLLEN              mIndicator;
        std_string          mName;
        std::vector<char>   mVecBuff;
        char                mCharBuff[BUFFER_SWITCH];
        const TypeRaw *     mTypeRaw;
        UDWORD              mDataSize;
    public:
        Field( const std_string name, const TypeRaw *data_type, UDWORD precision, SWORD scale, SWORD nullable );
        ~Field();
        SWORD ODBC_Type() const                         { return mTypeRaw->OdbcType; }
        SWORD C_Type() const                            { return mTypeRaw->OdbcCType; }
        ccdb::type DB_Type() const                      { return mTypeRaw->dbType; }
        UDWORD GetDataSize() const                      { return mDataSize; }
        const std_string& GetName() const               { return mName; }
        SQLPOINTER GetBuffer();
        SQLLEN GetBufferLength();
        SQLLEN * GetIndicatorAddress()                  { return &mIndicator; }
        bool IsNull() const                             { return mIndicator == SQL_NULL_DATA; }
        bool IsLongData() const                         { return ( mDataSize > LONG_DATA ); }
    };

    class HandleInfo
    {
    private:
        SQLHANDLE       mHandle;
        SQLSMALLINT     mHandleType;
    public:
        HandleInfo( SQLSMALLINT handle_type, SQLHANDLE handle )
            : mHandle( handle ), mHandleType( handle_type )
        {}

        SQLHANDLE Handle() const        { return mHandle; }
        SQLSMALLINT HandleType() const  { return mHandleType; }
    };

    //=======================================================================
    //======    Environment
    //=======================================================================
    class Environment
    {
    private:
        SQLHANDLE       mEnvironment;

        HandleInfo Hinfo()          { return HandleInfo( SQL_HANDLE_ENV, mEnvironment ); }
        // noncopyable
        Environment( const Environment& src ) CC_EQ_DELETE;
        Environment& operator=( const Environment& src ) CC_EQ_DELETE;
    public:
        Environment();
        ~Environment();
        SQLHANDLE GetHandle() const                         { return mEnvironment; }

        void SetOdbcVersion( unsigned long version );      // SQL_ATTR_ODBC_VERSION
    };

    //=======================================================================
    //======    Connection
    //=======================================================================
    class Connection
    {
    private:
        SQLHANDLE       mConnection;
        bool            mConnected;

        HandleInfo Hinfo()          { return HandleInfo( SQL_HANDLE_DBC, mConnection ); }
        // noncopyable
        Connection( const Connection& src ) CC_EQ_DELETE;
        Connection& operator=( const Connection& src ) CC_EQ_DELETE;
    public:
        Connection( Environment& env );
        ~Connection();
        SQLHANDLE GetHandle() const                                     { return mConnection; }
        void Connect( const std_string& connection_string );
        void Disconnect();
    };

    namespace detail
    {
        //=======================================================================
        //======    Statement
        //=======================================================================
        class Statement
        {
        private:
            SQLHANDLE               mOdbcHandle;
            std::vector<Field>      mFields;
            bool                    mIsEof;

            HandleInfo Hinfo()          { return HandleInfo( SQL_HANDLE_STMT, mOdbcHandle ); }
            // noncopyable
            Statement( const Statement& src ) CC_EQ_DELETE;
            Statement& operator=( const Statement& src ) CC_EQ_DELETE;
        public:
            Statement( Connection& connection );
            ~Statement();

            std::size_t GetFieldCount()                            { return mFields.size(); }

            std::tuple<std_string, int, ccdb::type>  // field name, data size, data type
            GetFieldAttributes( int idx );

            void ExecSql( const std_string& sql );
            void CloseSql();
            void Next();
            bool Eof() const                                    { return mIsEof; }

            Field * FieldByName( const std_string& field_name );
            Field * FieldByIndex( int idx )                     { return &mFields[idx]; }
        };
    }

    //=======================================================================
    //======    Select
    //=======================================================================
    class Select
    {
    private:
        detail::Statement       mStatement;
        // noncopyable
        Select( const Select& src ) CC_EQ_DELETE;
        Select& operator=( const Select& src ) CC_EQ_DELETE;
    public:
        Select( Connection& connection ) : mStatement( connection )             {}
        ~Select()                                                               {}

        std::size_t GetFieldCount()                                             { return mStatement.GetFieldCount(); }
        // field name, data size, data type
        std::tuple<std_string, int, ccdb::type> GetFieldAttributes( int idx )   { return mStatement.GetFieldAttributes( idx ); }
        void ExecSql( const std_string& sql )                                   { mStatement.ExecSql( sql ); }
        void CloseSql()                                                         { mStatement.CloseSql(); }
        void Next()                                                             { mStatement.Next(); }
        bool Eof() const                                                        { return mStatement.Eof(); }
        Field * FieldByName( const std_string& field_name )                     { return mStatement.FieldByName( field_name ); }
        Field * FieldByIndex( int idx )                                         { return mStatement.FieldByIndex( idx ); }
    };

    //=======================================================================
    //======    ExecSql
    //=======================================================================
    class ExecSql
    {
    private:
        detail::Statement       mStatement;
        // noncopyable
        ExecSql( const ExecSql& src ) CC_EQ_DELETE;
        ExecSql& operator=( const ExecSql& src ) CC_EQ_DELETE;
    public:
        ExecSql( Connection& connection ) : mStatement( connection )            {}
        ~ExecSql()                                                              {}

        void Exec( const std_string& sql )                                      { mStatement.ExecSql( sql ); }
        void CloseSql()                                                         { mStatement.CloseSql(); }
    };

    //=======================================================================
    //======    FREE functions
    //=======================================================================
    SQLRETURN CheckReturn( SQLRETURN ret, const HandleInfo& handle_info );
}

#endif
