//---------------------------------------------------------------------------
#ifndef CTreeH
#define CTreeH
//---------------------------------------------------------------------------
#include <iterator>
#include <ccList.h>

namespace cc
{

template <class T> class tlist
{
public:
	typedef	size_t		size_type;
	typedef	T			value_type;
	typedef	T*			pointer;
	typedef	T&			reference;
	typedef	const T*	const_pointer;
	typedef	const T&	const_reference;
	typedef	ptrdiff_t	difference_type;
private:

	template <class U> class ListNode;
	typedef ListNode<T>		*ListNodePtr;

	class ListNodeBase
	{
	public:
		ListNode<T>		*first, *last;
		ListNodeBase( ListNode<T> *src ) : first(src), last(src)		{}
	};

	template <class U> class ListNode : public ListNodeBase
	{
	public:
		U			value;
		ListNode( ListNode *src, const U& val )
			: ListNodeBase( src ), value(val)			{}
	};

	ListNodeBase	np;
	size_type		count;
protected:
	typedef std::iterator<std::bidirectional_iterator_tag, value_type,
						  difference_type, pointer,reference>					it;
	typedef std::iterator<std::bidirectional_iterator_tag, value_type,
						  difference_type, const_pointer, const_reference>		cit;
public:

#if ! defined ( S_DEBUG )
#pragma option push -vi
#endif

	class iterator;
	class const_iterator;

	class iterator : public it
	{
	private:
		friend class tlist<T>;
	protected:
		ListNodePtr		list_node_ptr;
	public:
		iterator() : list_node_ptr(0)											{}
		iterator( ListNodePtr ln_ptr ) : list_node_ptr( ln_ptr )				{}
		iterator( const iterator& src ) : list_node_ptr(src.list_node_ptr)		{}

		bool operator == ( const iterator& x ) const			{ return list_node_ptr == x.list_node_ptr; }
		bool operator != ( const iterator& x ) const			{ return !(*this == x); }
		reference operator * () const							{ return list_node_ptr->value; }
		pointer operator -> () const							{ return &(list_node_ptr->value); }
		iterator& operator ++ ()
		{
			list_node_ptr = list_node_ptr->last;
			return *this;
		}
		iterator operator ++ ( int )
		{
			iterator	tmp = *this;

			++*this;
			return tmp;
		}
		iterator& operator -- ()
		{
			list_node_ptr = list_node_ptr->first;
			return *this;
		}
		iterator operator -- ( int )
		{
			iterator	tmp = *this;

			--*this;
			return tmp;
		}
	};  // End of definition of iterator class.

	class const_iterator : public cit
	{
	private:
		friend class tlist<T>;
	protected:
		ListNodePtr		list_node_ptr;
	public:
		const_iterator() : list_node_ptr(0)											{}
		const_iterator( ListNodePtr ln_ptr ) : list_node_ptr( ln_ptr )				{}
		const_iterator( const iterator& src ) : list_node_ptr(src.list_node_ptr)	{}

		bool operator == ( const const_iterator& x ) const			{ return list_node_ptr == x.list_node_ptr; }
		bool operator != ( const const_iterator x ) const			{ return !(*this == x); }
		const_reference operator * () const							{ return list_node_ptr->value; }
		const_pointer operator -> () const							{ return &(list_node_ptr->data); }
		const_iterator& operator ++ ()
		{
			list_node_ptr = list_node_ptr->last;
			return *this;
		}
		const_iterator operator ++ ( int )
		{
			const_iterator	tmp = *this;

			++*this;
			return tmp;
		}
		const_iterator& operator -- ()
		{
			list_node_ptr = list_node_ptr->prev;
			return *this;
		}
		const_iterator operator -- ( int )
		{
			const_iterator	tmp = *this;

			--*this;
			return tmp;
		}
	};	// End of definition of const_iterator class.

#if ! defined ( S_DEBUG )
#pragma option pop
#endif

private:
	void link( iterator pos, ListNodePtr new_node );

// C++ Bulder 5 can compile this out of the class
#if ( __BORLANDC__ == 0x0550 || __BORLANDC__ == 0x0551 )
	ListNodePtr unlink( iterator pos );
#endif

// C++ Bulder 6 with or without Patch 1 cannot compile this out of the class
#if ( __BORLANDC__ == 0x0560 || __BORLANDC__ == 0x0561 )

#pragma option push -vi
	ListNodePtr unlink( iterator pos )
	{
		if ( pos.list_node_ptr == np.first )
			np.first = pos.list_node_ptr->last;
		if ( pos.list_node_ptr == np.last )
			np.last = pos.list_node_ptr->first;

		if ( pos.list_node_ptr->first != reinterpret_cast<ListNodePtr>(&np) )
			pos.list_node_ptr->first->last = pos.list_node_ptr->last;
		if ( pos.list_node_ptr->last != reinterpret_cast<ListNodePtr>(&np) )
			pos.list_node_ptr->last->first = pos.list_node_ptr->first;

		pos.list_node_ptr->first = reinterpret_cast<ListNodePtr>(&np);
		pos.list_node_ptr->last = reinterpret_cast<ListNodePtr>(&np);
		--count;

		return ( pos.list_node_ptr );
	}
#pragma option pop

#endif

public:
	tlist()
		: np( reinterpret_cast<ListNodePtr>(&np) ), count(0)
	{
	}

	tlist( const tlist<T>& src )
		: np( reinterpret_cast<ListNodePtr>(&np) ), count(0)
	{
		insert( end(), src.begin(), src.end() );
	}

	~tlist()
	{
		clear();
	}

	tlist<T>& operator=( const tlist<T>& src )
	{
		if ( this != &src )
			swap( tlist<T>( src ) );
	}

#if ! defined ( S_DEBUG )
#pragma option push -vi
#endif

	iterator begin()						{ return ( np.first ); }
	iterator end()							{ return ( np.last->last ); }
	iterator last()							{ return ( np.last ); }
	const_iterator begin() const			{ return ( np.first ); }
	const_iterator end() const				{ return ( np.last->last ); }
	size_type size()						{ return ( count ); }
	T& front()								{ return ( np.first->value ); }
	T& back()								{ return ( np.last->value ); }

#if ! defined ( S_DEBUG )
#pragma option pop
#endif

    void swap( tlist<T>& other )
    {
        ListNodeBase    tmp_ListNodeBase = other.np;
        size_type       tmp_count = other.count;

        if ( other.np.first != &other.np )
        {
            other.np.first->first = reinterpret_cast<ListNodePtr>(&np);
            other.np.last->last = reinterpret_cast<ListNodePtr>(&np);
        }
        if ( np.first != &np )
        {
            np.first->first = reinterpret_cast<ListNodePtr>(&other.np);
            np.last->last = reinterpret_cast<ListNodePtr>(&other.np);
        }
        other.np = np;
        other.count = count;
        np = tmp_ListNodeBase;
        count = tmp_count;
    }

#pragma warn -8027
	void clear()
	{
		iterator		start = begin(), eend = end();

		while ( start != eend )
			erase( start++ );
	}
#pragma warn +8027

	void erase( iterator pos )
	{
		delete unlink( pos );
	}

	void insert( iterator pos, const T& value )
	{
		ListNode<T>		*new_node = new ListNode<T>( reinterpret_cast<ListNodePtr>(&np), value );

		link( pos, new_node );
	}


#pragma warn -8027
	void insert( iterator pos, const_iterator start, const_iterator eend )
	{
		while ( start != eend )
		{
			insert( pos, *start );
			++start;
		}
	}
#pragma warn +8027

	void push_back( const T& x )									{ insert( end(), x ); }
};

#pragma option push -vi-

template <class T> void tlist<T>::link( iterator pos, ListNodePtr new_node )
{
	if ( count == 0 )
	{
		np.first = new_node;
		np.last = new_node;
	}
	else if ( pos.list_node_ptr->first == reinterpret_cast<ListNodePtr>(&np) )		/* first */
	{
		new_node->first = pos.list_node_ptr->first;
		new_node->last = pos.list_node_ptr;
		pos.list_node_ptr->first = new_node;
		np.first = new_node;
	}
	else if ( pos.list_node_ptr == reinterpret_cast<ListNodePtr>(&np) )				/* last + 1 */
	{
		new_node->last = pos.list_node_ptr;
		new_node->first = np.last;
		np.last->last = new_node;
		np.last = new_node;
	}
	else
	{
		new_node->first = pos.list_node_ptr->first;
		new_node->last = pos.list_node_ptr;
		pos.list_node_ptr->first->last = new_node;
		pos.list_node_ptr->first = new_node;
	}
	++count;
}

// C++ Bulder 5 can compile this out of the class
#if ( __BORLANDC__ == 0x0550 || __BORLANDC__ == 0x0551 )

template <class T> tlist<T>::ListNodePtr tlist<T>::unlink( iterator pos )
{
	if ( pos.list_node_ptr == np.first )
		np.first = pos.list_node_ptr->last;
	if ( pos.list_node_ptr == np.last )
		np.last = pos.list_node_ptr->first;

	if ( pos.list_node_ptr->first != reinterpret_cast<ListNodePtr>(&np) )
		pos.list_node_ptr->first->last = pos.list_node_ptr->last;
	if ( pos.list_node_ptr->last != reinterpret_cast<ListNodePtr>(&np) )
		pos.list_node_ptr->last->first = pos.list_node_ptr->first;

	pos.list_node_ptr->first = reinterpret_cast<ListNodePtr>(&np);
	pos.list_node_ptr->last = reinterpret_cast<ListNodePtr>(&np);
	--count;

	return ( pos.list_node_ptr );
}

#endif

#pragma option pop

template <class T> class tree
{
private:
	typedef	tlist<tree<T> >			Container;
public:
	typedef	Container::iterator		node;
	typedef	tree<T> *				pointer;
	typedef	Container::size_type    size_type;
private:
	T			FValue;
	pointer		FParent;
	Container	FNodes;
protected:
public:
	tree()
		: FValue(), FParent(NULL), FNodes()
    {
    }
	tree( const tree<T>& src )
		: FValue(src.FValue), FNodes(src.FNodes), FParent(src.FParent)
    {
    }
	tree( pointer parent, const T& item )
		: FValue(item), FParent(parent)
    {
    }

	~tree()
    {
    }

	tree& operator=( const tree<T>& src )
	{
		if ( this != &src )
		{
			FValue = src.FValue;
			FNodes = src.FNodes;
			FParent = src.FParent;
		}
		return ( *this );
	}

#if ! defined ( S_DEBUG )
#pragma option push -vi
#endif

	node begin()									{ return ( FNodes.begin() ); }
	node end()										{ return ( FNodes.end() ); }
	pointer parent()								{ return ( FParent ); }
	T& get_value()									{ return ( FValue ); }
    bool has_children()                             { return ( FNodes.size() != 0 ); }
	size_type size()						        { return ( FNodes.size() ); }

#if ! defined ( S_DEBUG )
#pragma option pop
#endif

	node add( const T& item )
	{
		FNodes.push_back( tree<T>( this, item ) );

		return ( FNodes.last() );
	}
};

template <class T> class CTree
{
private:
	typedef	CList< CTree< T > >		Container;
public:
	typedef	Container::iterator		node;
	typedef	CTree<T> *				pointer;
private:
	T			*FValue;
	pointer		FParent;
	Container	FNodes;
	CTree( const CTree<T>& src );
	CTree& operator=( const CTree<T>& src );
protected:
public:
	CTree()
		: FValue(NULL), FParent(NULL), FNodes()
    {
    }
	CTree( pointer parent, T *item )
		: FValue(item), FParent(parent)
    {
    }
	~CTree()
    {
        delete FValue;
    }

#if ! defined ( S_DEBUG )
#pragma option push -vi
#endif

    int size()                                      { return ( FNodes.GetCount() ); }
	node begin()									{ return ( FNodes.begin() ); }
	node end()										{ return ( FNodes.end() ); }
	pointer parent()								{ return ( FParent ); }
	T * get_value()									{ return ( FValue ); }
    bool has_children()                             { return ( FNodes.GetCount() != 0 ); }

#if ! defined ( S_DEBUG )
#pragma option pop
#endif

	node add( T *item )
	{
		FNodes.Add( new CTree<T>( const_cast<pointer>(this), item ) );

		return ( FNodes.begin() + (FNodes.GetCount() - 1) );
	}

    void clear()
    {
        FNodes.Clear();
    }
};

}; // end namespace cc

//---------------------------------------------------------------------------
#endif
