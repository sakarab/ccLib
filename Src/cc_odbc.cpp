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
#include "pre_cc.h"

#if defined (WIN32) || defined (__WIN32__) || defined (_WIN32)
    #include <windows.h>
#endif

#include "cc_odbc.h"
#include <stdexcept>
#include <boost/smart_ptr.hpp>
#include <cpp_lpstr.h>
#include <cc_array.hpp>

namespace ccodbc
{
    std::array<TypeRaw, 23>     DataTypeMap =
    {{
        { SQL_CHAR                      , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // CHAR(n) Character string of fixed string length n.
        { SQL_VARCHAR                   , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // VARCHAR(n) Variable-length character string with a maximum string length n.
        { SQL_LONGVARCHAR               , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // LONG VARCHAR Variable length character data. Maximum length is data source–dependent.[9]
        { SQL_WCHAR                     , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // WCHAR(n) Unicode character string of fixed string length n
        { SQL_WVARCHAR                  , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // VARWCHAR(n) Unicode variable-length character string with a maximum string length n
        { SQL_WLONGVARCHAR              , SQL_C_CHAR            , ccdb::type::ftString    , 0                            },  // LONGWVARCHAR Unicode variable-length character data. Maximum length is data source–dependent
        { SQL_DECIMAL                   , SQL_C_DOUBLE          , ccdb::type::ftDouble    , sizeof(double)               },  // DECIMAL(p,s) Signed, exact, numeric value with a precision of at least p and scale s. (The maximum precision is driver-defined.)(1 <= p <= 15; s <= p).[4]
        { SQL_NUMERIC                   , SQL_C_DOUBLE          , ccdb::type::ftDouble    , sizeof(double)               },  // NUMERIC(p,s) Signed, exact, numeric value with a precision p and scale s (1 <= p <= 15; s <= p).[4]
        { SQL_SMALLINT                  , SQL_C_SSHORT          , ccdb::type::ftShort     , sizeof(short)                },  // SMALLINT Exact numeric value with precision 5 and scale 0(signed:–32,768 <= n <= 32,767, unsigned:0 <= n <= 65,535)[3].
        { SQL_INTEGER                   , SQL_C_SLONG           , ccdb::type::ftInteger   , sizeof(int)                  },  // INTEGER Exact numeric value with precision 10 and scale 0(signed:–2[31] <= n <= 2[31] – 1, unsigned:0 <= n <= 2[32] – 1)[3].
        { SQL_REAL                      , SQL_C_DOUBLE          , ccdb::type::ftDouble    , sizeof(double)               },  // REAL Signed, approximate, numeric value with a binary precision 24 (zero or absolute value 10[–38] to 10[38]).
        { SQL_FLOAT                     , SQL_C_DOUBLE          , ccdb::type::ftDouble    , sizeof(double)               },  // FLOAT(p) Signed, approximate, numeric value with a binary precision of at least p. (The maximum precision is driver-defined.)[5]
        { SQL_DOUBLE                    , SQL_C_DOUBLE          , ccdb::type::ftDouble    , sizeof(double)               },  // DOUBLE PRECISION Signed, approximate, numeric value with a binary precision 53 (zero or absolute value 10[–308] to 10[308]).
        { SQL_BIT                       , SQL_C_BIT             , ccdb::type::ftBool      , sizeof(char)                 },  // BIT Single bit binary data.[8]
        { SQL_TINYINT                   , SQL_C_STINYINT        , ccdb::type::ftByte      , sizeof(char)                 },  // TINYINT Exact numeric value with precision 3 and scale 0(signed:–128 <= n <= 127,unsigned:0 <= n <= 255)[3].
        { SQL_BIGINT                    , SQL_C_SBIGINT         , ccdb::type::ftLongLong  , sizeof(long long)            },  // BIGINT Exact numeric value with precision 19 (if signed) or 20 (if unsigned) and scale 0(signed:–2[63] <= n <= 2[63] – 1,unsigned:0 <= n <= 2[64] – 1)[3],[9].
        { SQL_BINARY                    , SQL_C_BINARY          , ccdb::type::ftBlob      , 0                            },  // BINARY(n) Binary data of fixed length n.[9]
        { SQL_VARBINARY                 , SQL_C_BINARY          , ccdb::type::ftBlob      , 0                            },  // VARBINARY(n) Variable length binary data of maximum length n. The maximum is set by the user.[9]
        { SQL_LONGVARBINARY             , SQL_C_BINARY          , ccdb::type::ftBlob      , 0                            },  // LONG VARBINARY Variable length binary data. Maximum length is data source–dependent.[9]
        { SQL_TYPE_DATE                 , SQL_C_TYPE_DATE       , ccdb::type::ftDate      , sizeof(SQL_DATE_STRUCT)      },  // [6] DATE Year, month, and day fields, conforming to the rules of the Gregorian calendar. (See "Constraints of the Gregorian Calendar," later in this appendix.)
        { SQL_TYPE_TIME                 , SQL_C_TYPE_TIME       , ccdb::type::ftTime      , sizeof(SQL_TIME_STRUCT)      },  // [6] TIME(p) Hour, minute, and second fields, with valid values for hours of 00 to 23, valid values for minutes of 00 to 59, and valid values for seconds of 00 to 61. Precision p indicates the seconds precision.
        { SQL_TYPE_TIMESTAMP            , SQL_C_TYPE_TIMESTAMP  , ccdb::type::ftDateTime  , sizeof(SQL_TIMESTAMP_STRUCT) },  // [6] TIMESTAMP(p) Year, month, day, hour, minute, and second fields, with valid values as defined for the DATE and TIME data types.
        //{ SQL_TYPE_UTCDATETIME          , 0                     , 0                          , 0             },  // UTCDATETIME Year, month, day, hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10th microsecond precision.
        //{ SQL_TYPE_UTCTIME              , 0                     , 0                          , 0             },  // UTCTIME Hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10th microsecond precision..
        //{ SQL_INTERVAL_MONTH            , 0                     , 0                          , 0             },  // [7] INTERVAL MONTH(p) Number of months between two dates; p is the interval leading precision.
        //{ SQL_INTERVAL_YEAR             , 0                     , 0                          , 0             },  // [7] INTERVAL YEAR(p) Number of years between two dates; p is the interval leading precision.
        //{ SQL_INTERVAL_YEAR_TO_MONTH    , 0                     , 0                          , 0             },  // [7] INTERVAL YEAR(p) TO MONTH Number of years and months between two dates; p is the interval leading precision.
        //{ SQL_INTERVAL_DAY              , 0                     , 0                          , 0             },  // [7] INTERVAL DAY(p) Number of days between two dates; p is the interval leading precision.
        //{ SQL_INTERVAL_HOUR             , 0                     , 0                          , 0             },  // [7] INTERVAL HOUR(p) Number of hours between two date/times; p is the interval leading precision.
        //{ SQL_INTERVAL_MINUTE           , 0                     , 0                          , 0             },  // [7] INTERVAL MINUTE(p) Number of minutes between two date/times; p is the interval leading precision.
        //{ SQL_INTERVAL_SECOND           , 0                     , 0                          , 0             },  // [7] INTERVAL SECOND(p,q) Number of seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.
        //{ SQL_INTERVAL_DAY_TO_HOUR      , 0                     , 0                          , 0             },  // [7] INTERVAL DAY(p) TO HOUR Number of days/hours between two date/times; p is the interval leading precision.
        //{ SQL_INTERVAL_DAY_TO_MINUTE    , 0                     , 0                          , 0             },  // [7] INTERVAL DAY(p) TO MINUTE Number of days/hours/minutes between two date/times; p is the interval leading precision.
        //{ SQL_INTERVAL_DAY_TO_SECOND    , 0                     , 0                          , 0             },  // [7] INTERVAL DAY(p) TO SECOND(q) Number of days/hours/minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.
        //{ SQL_INTERVAL_HOUR_TO_MINUTE   , 0                     , 0                          , 0             },  // [7] INTERVAL HOUR(p) TO MINUTE Number of hours/minutes between two date/times; p is the interval leading precision.
        //{ SQL_INTERVAL_HOUR_TO_SECOND   , 0                     , 0                          , 0             },  // [7] INTERVAL HOUR(p) TO SECOND(q) Number of hours/minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.
        //{ SQL_INTERVAL_MINUTE_TO_SECOND , 0                     , 0                          , 0             },  // [7] INTERVAL MINUTE(p) TO SECOND(q) Number of minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.
        { SQL_GUID                      , SQL_C_GUID            , ccdb::type::ftGUID      , sizeof(SQLGUID)              }   // GUID Fixed length Globally Unique Identifier.
    }};

    //=======================================================================
    //======    FREE functions
    //=======================================================================
    const TypeRaw& TypeRawFromODBCtype( SWORD odbc_data_type )
    {
        for ( const TypeRaw& result : DataTypeMap )
            if ( result.OdbcType == odbc_data_type )
                return result;
        throw std::runtime_error( "ODBC: Unsupported database type." );
    }

    int DiagnosticCount( const HandleInfo& hinfo )
    {
        SQLINTEGER      result = 0;

        if ( SQLGetDiagField( hinfo.HandleType(), hinfo.Handle(), 0, SQL_DIAG_NUMBER, &result, 0, nullptr ) == SQL_SUCCESS )
            return result;
        return -1;
    }

    std::vector<std_string> DiagnosticMessageList( const HandleInfo& hinfo )
    {
        std_char        sql_state[10];
        SQLINTEGER      native_error;
        SQLSMALLINT     message_len_is;
        SQLSMALLINT     rec_no = 1;
        SQLRETURN       ret;

        std::array<std_char, 1024>      message;
        std::vector<std_string>         result;

        do
        {
            ret = SQLGetDiagRec( hinfo.HandleType(), hinfo.Handle(), rec_no, sql_state, &native_error,
                                 &message.front(), static_cast<SQLSMALLINT>(message.size()), &message_len_is );
            if ( ret == SQL_INVALID_HANDLE || ret == SQL_ERROR )
                break;

            std_string                  std_msg = std_string( &message.front() );

            result.push_back( std_msg );
            ++rec_no;
        }
        while ( ret != SQL_NO_DATA );
        return result;
    }

    SQLRETURN CheckReturn( SQLRETURN ret, const HandleInfo& handle_info )
    {
        if ( ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO )
            return ret;

        std::vector<std_string>     error_list = DiagnosticMessageList( handle_info );

        if ( ! error_list.empty() )
            throw std::runtime_error( cclib::narrow_string( error_list[0] ) );

        throw std::runtime_error( "ODBC Error." );
    }

    //=======================================================================
    //======    Field
    //=======================================================================
    Field::Field( const std_string name, const TypeRaw *data_type, UDWORD precision, SWORD scale, SWORD nullable )
        : mPrecision(precision), mScale(scale), mNullable(nullable), mIndicator(0),
          mName(name), mVecBuff()
    {
        // SQL type transilation. Better to provide an interface to handle it.
        if ( (data_type->OdbcType == SQL_DECIMAL || data_type->OdbcType == SQL_NUMERIC) && mScale == 0 && mPrecision <= 19 )
        {
            if ( mPrecision <= 3 )
                mTypeRaw = &TypeRawFromODBCtype( SQL_TINYINT );
            else if ( mPrecision <= 5 )
                mTypeRaw = &TypeRawFromODBCtype( SQL_SMALLINT );
            else if ( mPrecision <= 10 )
                mTypeRaw = &TypeRawFromODBCtype( SQL_INTEGER );
            else // if ( mPrecision <= 19 )
                mTypeRaw = &TypeRawFromODBCtype( SQL_BIGINT );
        }
        else
            mTypeRaw = data_type;

        if ( data_type->BinarySize != 0 )
            mDataSize = data_type->BinarySize;
        else
            mDataSize = precision;
        if ( mDataSize > BUFFER_SWITCH )
            mVecBuff.resize( mDataSize <= LONG_DATA ? mDataSize : LONG_DATA_BUFFER );
    }

    Field::~Field()
    {
    }

    SQLPOINTER Field::GetBuffer()
    {
        if ( mDataSize <= BUFFER_SWITCH )
            return mCharBuff;
        return &mVecBuff.front();
    }

    SQLLEN Field::GetBufferLength()
    {
        if ( mDataSize <= BUFFER_SWITCH )
            return BUFFER_SWITCH;
        return mVecBuff.size();
    }

    //=======================================================================
    //======    Environment
    //=======================================================================
    Environment::Environment()
        : mEnvironment(SQL_NULL_HANDLE)
    {
        CheckReturn( SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mEnvironment ), Hinfo() );
    }

    Environment::~Environment()
    {
        SQLFreeHandle( SQL_HANDLE_ENV, mEnvironment );
    }

    void Environment::SetOdbcVersion( unsigned long version )
    {
        CheckReturn( SQLSetEnvAttr( mEnvironment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(version), 0 ), Hinfo() );
    }

    //=======================================================================
    //======    Connection
    //=======================================================================
    Connection::Connection( Environment& env )
        : mConnection(SQL_NULL_HANDLE), mConnected(false)
    {
        CheckReturn( SQLAllocHandle( SQL_HANDLE_DBC, env.GetHandle(), &mConnection ), Hinfo() );
    }

    Connection::~Connection()
    {
        SQLFreeHandle( SQL_HANDLE_DBC, mConnection );
    }

    void Connection::Connect( const std_string& connection_string )
    {
        short                           new_len;
        cclib::array<std_char, 1024>    conn_str_out;

        CheckReturn( SQLDriverConnect( mConnection, 0, cclib::LPSTR( connection_string ).get(),
                                       static_cast<SQLSMALLINT>(connection_string.length()),
                                       conn_str_out.data(), static_cast<SQLSMALLINT>(conn_str_out.size())-1, &new_len, SQL_DRIVER_NOPROMPT ),
                     Hinfo() );
        mConnected = true;
    }

    void Connection::Disconnect()
    {
        if ( mConnected )
        {
            CheckReturn( SQLDisconnect( mConnection ), Hinfo() );
            mConnected = false;
        }
    }

    namespace detail
    {
        //=======================================================================
        //======    Statement
        //=======================================================================
        Statement::Statement( Connection& connection )
            : mOdbcHandle(SQL_NULL_HANDLE), mFields(), mIsEof(false)
        {
            CheckReturn( SQLAllocHandle( SQL_HANDLE_STMT, connection.GetHandle(), &mOdbcHandle ), Hinfo() );
        }

        Statement::~Statement()
        {
            SQLFreeHandle( SQL_HANDLE_STMT, mOdbcHandle );
        }

        std::tuple<std_string, int, ccdb::type>  // field name, data size, data type
        Statement::GetFieldAttributes( int idx )
        {
            Field&          field = mFields[idx];

            return std::make_tuple( field.GetName(), field.GetDataSize(), field.DB_Type() );
        }

        void Statement::ExecSql( const std_string& sql )
        {
            mIsEof = false;
            CheckReturn( SQLExecDirect( mOdbcHandle, cclib::LPSTR( sql ).get(), SQL_NTS ), Hinfo() );

            SWORD    nCols;                      // # of result columns

            CheckReturn( SQLNumResultCols( mOdbcHandle, &nCols ), Hinfo() );

            std::vector<std_char>   field_name( 50 );

            for ( SWORD n = 1 ; n <= nCols ; ++n )
            {
                SWORD   data_type;                  // column data type
                SQLULEN precision;                  // precision on the column
                SWORD   scale;                      // column scale
                SWORD   nullable;                   // nullable column ?
                SWORD   name_length;                // column data length

                SQLRETURN   ret = CheckReturn( SQLDescribeCol( mOdbcHandle, n, &field_name.front(), static_cast<SQLSMALLINT>(field_name.size()),
                                                               &name_length, &data_type, &precision, &scale, &nullable ), Hinfo() );
                if ( ret == SQL_SUCCESS_WITH_INFO )
                {
                    field_name.resize( name_length + 1 );
                    CheckReturn( SQLDescribeCol( mOdbcHandle, n, &field_name.front(), static_cast<SQLSMALLINT>(field_name.size()),
                                                 &name_length, &data_type, &precision, &scale, &nullable ), Hinfo() );
                }
                mFields.push_back( Field( std_string( &field_name.front(), name_length ), &TypeRawFromODBCtype( data_type ), precision, scale, nullable ) );
            }
            for ( SWORD n = 1 ; n <= nCols ; ++n )
            {
                Field&     field = mFields[n-1];

                if ( ! field.IsLongData() )
                    CheckReturn( SQLBindCol( mOdbcHandle, n, field.C_Type(), field.GetBuffer(),
                                             field.GetBufferLength(), field.GetIndicatorAddress() ), Hinfo() );
            }
            Next();
        }

        void Statement::CloseSql()
        {
            CheckReturn( SQLFreeStmt( mOdbcHandle, SQL_CLOSE ), Hinfo() );
            mIsEof = false;
        }

        void Statement::Next()
        {
            SQLRETURN   nReturn = SQLFetch( mOdbcHandle );

            if ( nReturn != SQL_NO_DATA )
                CheckReturn( nReturn, Hinfo() );
            mIsEof = nReturn != SQL_SUCCESS;
        }

        Field * Statement::FieldByName( const std_string& field_name )
        {
            for ( std::vector<Field>::iterator it = mFields.begin(), eend = mFields.end() ; it != eend ; ++it )
                if ( it->GetName() == field_name )
                    return &(*it);
            throw std::runtime_error( "ODBC FieldByName error." );
        }
    }
}
