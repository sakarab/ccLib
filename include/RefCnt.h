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

#ifndef SM_RefCnt_H_INCLUDED
#define SM_RefCnt_H_INCLUDED

#include "predef_cc.h"          // there is no support for BCB

namespace cclib
{

template <class T> struct tArg00
{
    typedef T   type;
};

template <class T, class Arg01> struct tArg01
{
    typedef T       type;
    typedef Arg01   arg01;
};

template <class T, class Arg01, class Arg02> struct tArg02
{
    typedef T       type;
    typedef Arg01   arg01;
    typedef Arg02   arg02;
};

template <class T> class Handle
{
private:
	Handle<T> * operator& () CC_EQ_DELETE;          // Not defined to prevent usage.

	template <class U> class Counted
	{
	private:
		U				instance_;
		unsigned int	num_references_;
	public:
		// Preferred form of T instance creation.
		Counted() : instance_(), num_references_(1)                             {} // empty

		template <class Arg01> Counted( const Arg01& arg01 )
            : instance_( arg01 ), num_references_(1)
        {
        }
		template <class Arg01, class Arg02> Counted( const Arg01& arg01, const Arg02& arg02 )
            : instance_( arg01, arg02 ), num_references_(1)
        {
        }

		// Should be avoided as an internal copy of 'from' is created.
        Counted( const U& from ) : instance_(from), num_references_(1)          {} // empty
		~Counted()													            {} // empty

		void dismiss()
        {
            if ( !--num_references_ )
                delete this;
        }
		void use()						{ ++num_references_; }
        U& get_instance()               { return  instance_; }
        const U& get_instance() const   { return  instance_; }
        //operator         U& ()       	{ return  instance_; }
        //operator   const U& () const 	{ return  instance_; }
        operator         U* ()       	{ return &instance_; }
        operator   const U* () const 	{ return &instance_; }
        U*       operator-> ()       	{ return &instance_; }
        const U* operator-> () const 	{ return &instance_; }
		bool is_shared() const			{ return num_references_ > 1; }
	};

	Counted<T>	*counted_;

    void FreeCounted()
    {
        counted_->dismiss();
        counted_ = 0;
    }
public:
	Handle() : counted_( new Counted<T>() )	                                    {} // empty
	Handle( const T& copy ) : counted_( new Counted( copy ) )	                {} // empty
// sam. Removed as it is dengerous
//    Handle( const T *copy )
//        : counted_( copy ? new Counted( *copy ) : new Counted() )
//    {
//    }
	Handle( const Handle<T>& ref ) : counted_( ref.counted_ )	                { counted_->use(); }

    template <class Arg01> Handle( const Arg01& arg01 )
        : counted_( new Counted<T>( arg01 ) )
    {
    }
    template <class Arg01, class Arg02> Handle( const Arg01& arg01, const Arg02& arg02 )
        : counted_( new Counted<T>( arg01, arg02 ) )
    {
    }

	~Handle()                                                                   { counted_->dismiss(); }

	operator T& ()						{ return counted_->operator T&(); }
	operator T* ()						{ return counted_->operator T*(); }
	operator const T& () const			{ return counted_->operator T&(); }
	operator const T* () const			{ return counted_->operator T*(); }
	const T* operator-> () const		{ return counted_->operator ->(); }
	T* operator-> ()					{ return counted_->operator ->(); }

	Handle<T>& operator = ( const T& src )
    {
        if ( this != &src )
        {
            FreeCounted();
            counted_ = new Counted( src );
        }
        return *this;
    }
	Handle<T>& operator = ( const T* src )
    {
        if ( *this != src )
        {
            FreeCounted();
            counted_ = src ? new Counted( *src ) : new Counted();
        }
        return *this;
    }

	Handle<T>& operator = ( const Handle<T>& src )
    {
        if ( this->counted_ != src.counted_ )
        {
            counted_->dismiss();
            counted_ = src.counted_;
            counted_->use();
        }
        return *this;
    }
	bool is_shared() const						{ return counted_->is_shared(); }

	// Convenience method to create a copy of a "T-Handle<T>" pair.
	Handle<T> duplicate() const					{ return Handle<T>( operator->() ); }
};

/*
template<class T> inline Handle<T>& Handle<T>::operator=( const T& src )  //8.
{
	if ( this != &src )
	{
        FreeCounted();
		counted_ = new Counted( src );
	}
	return *this;
}

template<class T> inline Handle<T>& Handle<T>::operator=( const T* src ) //8.
{
	if ( *this != src )
	{
        FreeCounted();
		counted_ = src ? new Counted( *src ) : new Counted();
	}
	return *this;
}

template<class T> inline Handle<T>& Handle<T>::operator=( const Handle<T>& src )
{
	if ( this->counted_ != src.counted_ )
	{
		counted_->dismiss();
		counted_ = src.counted_;
		counted_->use();
	}
	return *this;
}
*/

}; // end namespace cclib
//---------------------------------------------------------------------------

#endif
