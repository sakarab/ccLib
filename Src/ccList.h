//---------------------------------------------------------------------------
#ifndef ccListH
#define ccListH
//---------------------------------------------------------------------------
#include "ssport.h"
#include <cstddef>
#include <memory>

namespace cc
{

//#if ! defined ( S_DEBUG )
//#pragma option push -vi
//#endif

class CCustomList
{
public:
    typedef int         size_type;
private:
    enum { MAX_LIST_SIZE = 0x7FFFFFFF / 16 };

	void **				FList;
	int					FCount;
	int					FCapacity;
	bool                FCanDelete;
    //static void FASTCALL Error( int res_id, int data );
    static void FASTCALL OutOfRangeError();
    static void FASTCALL RangeError();
	int FASTCALL CheckIndex( int Index )
	{
#if defined ( S_DEBUG )
		if ( Index < 0 || Index >= FCount )
			OutOfRangeError();
#endif
		return ( Index );
	}
	NO_COPY_CTOR(CCustomList);
	NO_COPY_OPER(CCustomList);
protected:
	virtual void FASTCALL Grow( void );
	virtual void DeleteItem( void *Item ) = 0;
	FASTCALL CCustomList( bool can_delete = true );
	FASTCALL ~CCustomList( void );
	// Override	-------------------------------------------------------
	void * FASTCALL Get( int Index ) const
	{
#if defined ( S_DEBUG )
		CheckIndex( Index );
#endif
		return ( GetList()[Index] );
	}
	void FASTCALL Put( int Index, void * Item );
	int FASTCALL Add( void *Item );
	void * FASTCALL First( void );
	int FASTCALL IndexOf( void *Item );
	void FASTCALL Insert( int Index, void *Item );
	void * FASTCALL Last( void );
	int FASTCALL Remove( void *Item );
	CCustomList* FASTCALL Expand( void );			// ??
	// end Override	---------------------------------------------------
	void ** FASTCALL GetList() const                                { return ( FList ); }
public:
	void FASTCALL Clear( void );
	void FASTCALL Delete( int Index );
	void FASTCALL Exchange( int Index1, int Index2 );
	void FASTCALL Pack( void );
	void FASTCALL Move( int CurIndex, int NewIndex );
	void FASTCALL SetCapacity( int NewCapacity );
	void FASTCALL SetCount( int NewCount );
	int FASTCALL GetCapacity()										{ return ( FCapacity ); }
	int FASTCALL GetCount() const                                   { return ( FCount ); }
};

template <class T> class CList : public CCustomList
{
public:
	//typedef	std::size_t		size_type;
	typedef	T               value_type;
	typedef	T*			    pointer;
	typedef	T&			    reference;
	typedef	const T*	    const_pointer;
	typedef	const T&	    const_reference;
	typedef	std::ptrdiff_t  difference_type;
private:
	T ** FASTCALL GetList()						{ return ( reinterpret_cast<T **>(CCustomList::GetList()) ); }
	NO_COPY_CTOR(CList);
	NO_COPY_OPER(CList);
protected:
	virtual void DeleteItem( void *Item )		{ delete reinterpret_cast<pointer>(Item); }

	template <class FUNC> void FASTCALL QuickSort( T **SortList, int L, int R, const FUNC& SCompare )
	{
		int		I, J;
		T		*p, *t;

		do
		{
			I = L;
			J = R;
			p = SortList[(L + R) >> 1];
			do
			{
				while ( SCompare( SortList[I], p ) < 0 )
					I++;
				while ( SCompare( SortList[J], p ) > 0 )
					J--;
				if ( I <= J )
				{
					t = SortList[I];
					SortList[I] = SortList[J];
					SortList[J] = t;
					I++;
					J--;
				}
			}
			while ( I <= J );
			if ( L < J )
				QuickSort( SortList, L, J, SCompare );
			L = I;
		}
		while ( I < R );
	}

protected:
	typedef std::iterator<std::random_access_iterator_tag, value_type,
						  difference_type, pointer, reference>					it;
	typedef std::iterator<std::random_access_iterator_tag, value_type,
						  difference_type, const_pointer, const_reference>		cit;
public:

//#if ! defined ( S_DEBUG )
//#pragma option push -vi
//#endif

	class iterator;
	class const_iterator;

	class iterator : public it
	{
	protected:
		T	**flist;
	public:
		iterator() : flist(NULL)									{}
		iterator( T **list ) : flist(list)							{}
		iterator( const iterator& src ) : flist(src.flist)			{}
		void operator = ( const iterator& x )						{ flist = x.flist; }

		bool operator == ( const iterator& x ) const				{ return flist == x.flist; }
		bool operator != ( const iterator& x ) const				{ return !(*this == x); }
		reference operator * () const								{ return ( **flist ); }
		pointer operator -> () const								{ return ( *flist ); }
		iterator& operator ++ ()									{ ++flist; return *this; }
		iterator operator ++ ( int )
		{
			iterator	result = *this;

			++*this;
			return result;
		}
		iterator& operator -- ()									{ --flist; return *this; }
		iterator operator -- ( int )
		{
			iterator	result = *this;

			--*this;
			return result;
		}
		iterator operator + ( difference_type diff ) const
		{
			iterator	result = *this;
			return ( result += diff );
		}
		iterator operator - ( difference_type diff ) const
		{
			iterator	result = *this;
			return ( result -= diff );
		}
		difference_type operator - ( const iterator& it1 ) const	{ return ( flist - it1.flist ); }
		bool operator < ( const iterator& it1 ) const				{ return ( flist < it1.flist ); }
		bool operator > ( const iterator& x ) const					{ return ( flist > it1.flist ); }
		bool operator >= ( const iterator& x ) const				{ return ( flist >= it1.flist ); }
		bool operator <= ( const iterator& x ) const				{ return ( flist <= it1.flist ); }
		iterator& operator += ( difference_type diff )				{ flist += diff; return *this; }
		iterator& operator -= ( difference_type diff )				{ flist -= diff; return *this; }

		reference operator [] ( size_type idx )						{ return ( *(flist[idx]) ); }
	};  // End of definition of iterator class.

	class const_iterator : public cit
	{
	protected:
		T	**flist;
	public:
		const_iterator() : flist(NULL)									{}
		const_iterator( T **list ) : flist(list)						{}
		const_iterator( const const_iterator& src ) : flist(src.flist)	{}
		void operator = ( const const_iterator& x )					  	{ flist = x.flist; }

		bool operator == ( const const_iterator& x ) const				{ return flist == x.flist; }
		bool operator != ( const const_iterator x ) const				{ return !(*this == x); }
		const_reference operator * () const								{ return ( **flist ); }
		const_pointer operator -> () const								{ return ( *flist ); }
		const_iterator& operator ++ ()									{ ++flist; return *this; }
		const_iterator operator ++ ( int )
		{
			const_iterator	result = *this;

			++*this;
			return result;
		}
		const_iterator& operator -- ()									{ --flist; return *this; }
		const_iterator operator -- ( int )
		{
			const_iterator	result = *this;

			--*this;
			return result;
		}
		const_iterator operator + ( difference_type diff ) const
		{
			const_iterator	result = *this;
			return ( result += diff );
		}
		const_iterator operator - ( difference_type diff ) const
		{
			const_iterator	result = *this;
			return ( result -= diff );
		}
		difference_type operator - ( const const_iterator& it1 ) const	{ return ( flist - it1.flist ); }
		bool operator < ( const const_iterator& it1 ) const				{ return ( flist < it1.flist ); }
		bool operator > ( const const_iterator& x ) const				{ return ( flist > it1.flist ); }
		bool operator >= ( const const_iterator& x ) const				{ return ( flist >= it1.flist ); }
		bool operator <= ( const const_iterator& x ) const				{ return ( flist <= it1.flist ); }
		const_iterator& operator += ( difference_type diff )			{ flist += diff; return *this; }
		const_iterator& operator -= ( difference_type diff )			{ flist -= diff; return *this; }

		const_reference operator [] ( size_type idx )					{ return ( *(flist[idx]) ); }
	};	// End of definition of const_iterator class.

//#if ! defined ( S_DEBUG )
//#pragma option pop
//#endif

public:
	FASTCALL CList( bool can_delete = true ) : CCustomList( can_delete )    {}
	FASTCALL ~CList( void )                                                 { Clear(); }
	T * FASTCALL GetItem( int Index ) const                                 { return ( reinterpret_cast<pointer>(CCustomList::Get( Index )) ); }
	void FASTCALL SetItem( int Index, pointer Item )                        { CCustomList::Put( Index, Item ); }
	int FASTCALL Add( pointer Item )                                        { return ( CCustomList::Add( Item ) ); }
	T * FASTCALL First( void )                                              { return ( reinterpret_cast<pointer>(CCustomList::First()) ); }
	int FASTCALL IndexOf( pointer Item )                                    { return ( CCustomList::IndexOf( Item ) ); }
	void FASTCALL Insert( int Index, pointer Item )                         { CCustomList::Insert( Index, Item ); }
	T * FASTCALL Last( void )                                               { return ( reinterpret_cast<pointer>(CCustomList::Last()) ); }
	int FASTCALL Remove( pointer Item )                                     { return ( CCustomList::Remove( Item ) ); }

	iterator begin()                                                        { return ( GetList() ); }
	iterator end()                                                          { return ( GetList() + GetCount() ); }

	template <class FUNC> void FASTCALL Sort( const FUNC& func )
	{
		if ( GetList() != NULL && GetCount() > 0 )
			QuickSort( GetList(), 0, GetCount() - 1, func );
	}
	template <class WHAT, class FUNC> T * FASTCALL Find( const WHAT& what, const FUNC& func, int& idx )
	{
		int		L, H, I, C;

		L = 0;
		H = GetCount() - 1;
		while ( L <= H )
		{
			I = (L + H) >> 1;
			C = func( what, GetList()[I] );
			if ( C < 0 )
				L = I + 1;
			else if ( C > 0 )
				H = I - 1;
			else
			{
				idx = I;
				return ( GetList()[I] );
			}
		}
		idx = L;
		return ( NULL );
	}
	template <class WHAT, class FUNC> T * FASTCALL Find( const WHAT& what, const FUNC& func )
	{
		int		idx;
		return ( Find( what, func, idx ) );
	}
};

//#if ! defined ( S_DEBUG )
//#pragma option pop
//#endif

}; // end namespace cc
//---------------------------------------------------------------------------
#endif

