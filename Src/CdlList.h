//---------------------------------------------------------------------------
#ifndef CdlListH
#define CdlListH
//---------------------------------------------------------------------------
#include "ssport.h"

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
	FASTCALL CdlNode();
	FASTCALL ~CdlNode();
	void FASTCALL LinkPrev( CdlNode *new_node );		// new node becomes PREVIOUS
	void FASTCALL LinkNext( CdlNode *new_node );		// new node becomes NEXT
	void FASTCALL Unlink( void );
	CdlNode * FASTCALL Delete( void );
	void FASTCALL Sort( long lo, long hi, CdlNode *nlo, CdlNode *nhi, TdlCustomCompareFunc func );
	CdlNode * FASTCALL MoveUpBy( long cnt );
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
	CdlNode * FASTCALL GetNode( long index );
protected:
	FASTCALL CCustomDLList( bool can_delete = true );
	FASTCALL ~CCustomDLList();
	virtual void FASTCALL DeleteItem( void *a_object ) = 0;
	void FASTCALL LinkNode( long pos, CdlNode *new_node );
	void FASTCALL UnlinkNode( CdlNode *node );
	// Override	-------------------------------------------------------
	void * FASTCALL Add( void *a_object );
	void * FASTCALL InsertAtPos( void *a_object, long pos );
	long FASTCALL PosFromObject( void *a_object );
	void * _fastcall Find( void *what, TdlFindFunc func );
	void * _fastcall FindGreater( void *what, TdlFindFunc func );
	void * _fastcall FindLess( void *what, TdlFindFunc func );
	// end Override	---------------------------------------------------
	void FASTCALL Clear( void );
	void * FASTCALL IteratorFirst( void );
	void * FASTCALL IteratorLast( void );
	void * FASTCALL IteratorNext( void );
	void * FASTCALL IteratorPrev( void );
	void * FASTCALL operator []( long index );
	void FASTCALL QSort( TdlCustomCompareFunc func );
	void FASTCALL MoveToList( CCustomDLList *list, long from_pos, long to_pos );
	__property CdlNode *Nodes[long index] = { read=GetNode };
public:
	void FASTCALL DeleteAtPos( long pos );
	void FASTCALL Swap( long pos1, long pos2 );
	void FASTCALL Move( long from_pos, long to_pos );
	__property long Count = { read=FCount };
	__property bool CanDelete = { read=FCanDelete, write=FCanDelete };
};
//---------------------------------------------------------------------------
template <class T> class TdlList : public CCustomDLList
{
protected:
	virtual void FASTCALL DeleteItem( void *a_object )
	{
		delete ((T *)a_object);
	}
public:
	typedef int (*TdlCompareFunc)( T *a_object, T *b_object );

	FASTCALL TdlList( bool can_delete=true ) : CCustomDLList( can_delete )
	{
	}
	FASTCALL ~TdlList()
	{
		Clear();
	}
	T * FASTCALL Add( T *a_object )
	{
		return (T *)(CCustomDLList::Add( (TObject *)a_object ));
	}
	T * FASTCALL InsertAtPos( T *a_object, long pos )
	{
		return (T *)(CCustomDLList::InsertAtPos( a_object, pos ));
	}
	long FASTCALL PosFromObject( T *a_object )
	{
		return (CCustomDLList::PosFromObject( a_object ));
	}
	T * FASTCALL IteratorFirst( void )
	{
		return (T *)(CCustomDLList::IteratorFirst());
	}
	T * FASTCALL IteratorLast( void )
	{
		return (T *)(CCustomDLList::IteratorLast());
	}
	T * FASTCALL IteratorNext( void )
	{
		return (T *)(CCustomDLList::IteratorNext());
	}
	T * FASTCALL IteratorPrev( void )
	{
		return (T *)(CCustomDLList::IteratorPrev());
	}
	void FASTCALL QSort( TdlCompareFunc func )
	{
		CCustomDLList::QSort( (TdlCustomCompareFunc)func );
	}
	T * FASTCALL operator []( long index )
	{
		return (T *)(CCustomDLList::operator []( index ));
	}
};
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
#endif
