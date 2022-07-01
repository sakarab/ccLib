#include "stdafx.h"
#include <boost/format.hpp>

#include "cclib_tests.h"

namespace
{
    template <class T> T rotate_l_3( T value, unsigned int count )
    {
        const unsigned int      mask = std::numeric_limits<T>::digits - 1;  // assumes width is a power of 2.

        return (value << count) | (value >> ((~count + 1) & mask));
    }

    template <class T> T rotate_r_3( T value, unsigned int count )
    {
        const unsigned int      mask = std::numeric_limits<T>::digits - 1;  // assumes width is a power of 2.

        return (value >> count) | (value << ((~count + 1) & mask));
    }

    template <class T>          T bits_count( T value )                     { return sizeof(value) * std::numeric_limits<T>::digits; }
    template <class T, class S> T shift_count( T value, S steps )           { return steps % bits_count( value ); }
    template <class T, class S> T reverse_shift_count( T value, S steps )   { return bits_count( value ) - shift_count( value, steps ); }

    template <class T> T rotate_l_2( T value, unsigned int count )
    {
        return (value << shift_count( value, count ) | (value >> reverse_shift_count( value, count ) ) );
    }

    template <class T> T rotate_r_2( T value, unsigned int count )
    {
        return (value >> shift_count( value, count ) | (value << reverse_shift_count( value, count ) ) );
    }

    template <class T> T rotate_l( T value, unsigned int count )
    {
        static_assert( std::is_unsigned<T>::value, "Rotate Left only makes sense for unsigned types" );
        return (value << count) | (value >> (sizeof(T) * std::numeric_limits<T>::digits - count));
    }

    template <class T> T rotate_r( T value, unsigned int count )
    {
        static_assert( std::is_unsigned<T>::value, "Rotate Left only makes sense for unsigned types" );
        return (value >> count) | (value << (sizeof(T) * std::numeric_limits<T>::digits - count));
    }



}

void testBase64( CRichEditCtrl& logger )
{
    unsigned int    value = 0x80808003;

    logger.AppendText( boost::str( boost::wformat( L"Value-0 = %X\r\n" ) % value ).c_str() );

    value = rotate_l_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-1 = %X\r\n" ) % value ).c_str() );
    value = rotate_l_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-2 = %X\r\n" ) % value ).c_str() );
    value = rotate_l_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-3 = %X\r\n" ) % value ).c_str() );
    value = rotate_l_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-4 = %X\r\n" ) % value ).c_str() );

    value = rotate_r_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-3 = %X\r\n" ) % value ).c_str() );
    value = rotate_r_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-2 = %X\r\n" ) % value ).c_str() );
    value = rotate_r_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-1 = %X\r\n" ) % value ).c_str() );
    value = rotate_r_3( value, 8 );
    logger.AppendText( boost::str( boost::wformat( L"Value-0 = %X\r\n" ) % value ).c_str() );
}
