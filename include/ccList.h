//---------------------------------------------------------------------------
#ifndef ccListH
#define ccListH
//---------------------------------------------------------------------------
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
    //static void Error( int res_id, int data );
    static void OutOfRangeError();
    static void RangeError();
	int CheckIndex( int Index )
	{
#if defined ( S_DEBUG )
		if ( Index < 0 || Index >= FCount )
			OutOfRangeError();
#endif
		return ( Index );
	}
	CCustomList( const CCustomList& );
	CCustomList& operator=( const CCustomList& );
protected:
	virtual void Grow( void );
	virtual void DeleteItem( void *Item ) = 0;
	CCustomList( bool can_delete = true );
	~CCustomList( void );
	// Override	-------------------------------------------------------
	void * Get( int Index ) const
	{
#if defined ( S_DEBUG )
		CheckIndex( Index );
#endif
		return ( GetList()[Index] );
	}
	void Put( int Index, void * Item );
	int Add( void *Item );
	void * First( void );
	int IndexOf( void *Item );
	void Insert( int Index, void *Item );
	void * Last( void );
	int Remove( void *Item );
	CCustomList* Expand( void );			// ??
	// end Override	---------------------------------------------------
	void ** GetList() const                                { return ( FList ); }
public:
	void Clear( void );
	void Delete( int Index );
	void Exchange( int Index1, int Index2 );
	void Pack( void );
	void Move( int CurIndex, int NewIndex );
	void SetCapacity( int NewCapacity );
	void SetCount( int NewCount );
	int GetCapacity()										{ return ( FCapacity ); }
	int GetCount() const                                   { return ( FCount ); }
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
	T ** GetList()						{ return ( reinterpret_cast<T **>(CCustomList::GetList()) ); }
    // non-copyable
    CList( const CList& );
    CList& operator=( const CList& );
protected:
	virtual void DeleteItem( void *Item )		{ delete reinterpret_cast<pointer>(Item); }

	template <class FUNC> void QuickSort( T **SortList, int L, int R, const FUNC& SCompare )
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
	CList( bool can_delete = true ) : CCustomList( can_delete )    {}
	~CList( void )                                                 { Clear(); }
	T * GetItem( int Index ) const                                 { return ( reinterpret_cast<pointer>(CCustomList::Get( Index )) ); }
	void SetItem( int Index, pointer Item )                        { CCustomList::Put( Index, Item ); }
	int Add( pointer Item )                                        { return ( CCustomList::Add( Item ) ); }
	T * First( void )                                              { return ( reinterpret_cast<pointer>(CCustomList::First()) ); }
	int IndexOf( pointer Item )                                    { return ( CCustomList::IndexOf( Item ) ); }
	void Insert( int Index, pointer Item )                         { CCustomList::Insert( Index, Item ); }
	T * Last( void )                                               { return ( reinterpret_cast<pointer>(CCustomList::Last()) ); }
	int Remove( pointer Item )                                     { return ( CCustomList::Remove( Item ) ); }

	iterator begin()                                                        { return ( GetList() ); }
	iterator end()                                                          { return ( GetList() + GetCount() ); }

	template <class FUNC> void Sort( const FUNC& func )
	{
		if ( GetList() != NULL && GetCount() > 0 )
			QuickSort( GetList(), 0, GetCount() - 1, func );
	}
	template <class WHAT, class FUNC> T * Find( const WHAT& what, const FUNC& func, int& idx )
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
	template <class WHAT, class FUNC> T * Find( const WHAT& what, const FUNC& func )
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

