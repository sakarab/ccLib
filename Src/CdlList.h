//---------------------------------------------------------------------------
#ifndef CdlListH
#define CdlListH
//---------------------------------------------------------------------------
#if ( __BORLANDC__ >= 0x0560 )          // C++ Bulder 6
    #include <rtlConsts.hpp>
#elif ( __BORLANDC__ >= 0x0550 )        // C++ Bulder 5
    #include <Consts.hpp>
#endif

#include <memory>

typedef int (*TdlCustomCompareFunc)( void *a_object, void *b_object );
typedef int (*TdlFindFunc)( void *what, void *a_object );

class CdlNode
{
	friend class CCustomDLList;
private:
	CdlNode	*FPrev;
	CdlNode	*FNext;
	void	*FData;
protected:
	CdlNode();
	~CdlNode();
	void LinkPrev( CdlNode *new_node );		// new node becomes PREVIOUS
	void LinkNext( CdlNode *new_node );		// new node becomes NEXT
	void Unlink( void );
	CdlNode * Delete( void );
	void Sort( long lo, long hi, CdlNode *nlo, CdlNode *nhi, TdlCustomCompareFunc func );
	CdlNode * MoveUpBy( long cnt );
	__property void *Data = { read=FData };
public:
};
//---------------------------------------------------------------------------
class CCustomDLList
{
private:
	long		FCount;
	CdlNode		*FFirst;
	CdlNode		*FLast;
	bool		FCanDelete;
	CdlNode *	FIteratorCurrent;
	CdlNode *   GetNode( long index );
protected:
	CCustomDLList( bool can_delete = true );
	~CCustomDLList();
	virtual void DeleteItem( void *a_object ) = 0;
	void LinkNode( long pos, CdlNode *new_node );
	void UnlinkNode( CdlNode *node );
	// Override	-------------------------------------------------------
	void * Add( void *a_object );
	void * InsertAtPos( void *a_object, long pos );
	long PosFromObject( void *a_object );
	void * _fastcall Find( void *what, TdlFindFunc func );
	void * _fastcall FindGreater( void *what, TdlFindFunc func );
	void * _fastcall FindLess( void *what, TdlFindFunc func );
	// end Override	---------------------------------------------------
	void Clear( void );
	void * IteratorFirst( void );
	void * IteratorLast( void );
	void * IteratorNext( void );
	void * IteratorPrev( void );
	void * operator []( long index );
	void QSort( TdlCustomCompareFunc func );
	void MoveToList( CCustomDLList *list, long from_pos, long to_pos );
	__property CdlNode *Nodes[long index] = { read=GetNode };
public:
	void DeleteAtPos( long pos );
	void Swap( long pos1, long pos2 );
	void Move( long from_pos, long to_pos );
	__property long Count = { read=FCount };
	__property bool CanDelete = { read=FCanDelete, write=FCanDelete };
};
//---------------------------------------------------------------------------
template <class T> class TdlList : public CCustomDLList
{
protected:
	virtual void DeleteItem( void *a_object )
	{
		delete ((T *)a_object);
	}
public:
	typedef int (*TdlCompareFunc)( T *a_object, T *b_object );

	TdlList( bool can_delete=true ) : CCustomDLList( can_delete )
	{
	}
	~TdlList()
	{
		Clear();
	}
	T * Add( T *a_object )
	{
		return (T *)(CCustomDLList::Add( (TObject *)a_object ));
	}
	T * InsertAtPos( T *a_object, long pos )
	{
		return (T *)(CCustomDLList::InsertAtPos( a_object, pos ));
	}
	long PosFromObject( T *a_object )
	{
		return (CCustomDLList::PosFromObject( a_object ));
	}
	T * IteratorFirst( void )
	{
		return (T *)(CCustomDLList::IteratorFirst());
	}
	T * IteratorLast( void )
	{
		return (T *)(CCustomDLList::IteratorLast());
	}
	T * IteratorNext( void )
	{
		return (T *)(CCustomDLList::IteratorNext());
	}
	T * IteratorPrev( void )
	{
		return (T *)(CCustomDLList::IteratorPrev());
	}
	void QSort( TdlCompareFunc func )
	{
		CCustomDLList::QSort( (TdlCustomCompareFunc)func );
	}
	T * operator []( long index )
	{
		return (T *)(CCustomDLList::operator []( index ));
	}
};
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
#endif
