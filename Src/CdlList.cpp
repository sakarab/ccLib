//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Classes.hpp>
#include <stdlib>
#include "CdlList.h"
#include "ccUtils.h"
//---------------------------------------------------------------------------

/*
static void Error( int res_id, int data )
{
	AnsiString	mess;

	mess.LoadStr( res_id );
	throw Exception( mess.cat_sprintf( " %d", data ) );
}
*/

//---------------------------------------------------------------------------

FASTCALL CdlNode::CdlNode()
{
	FNext = NULL;
	FPrev = NULL;
	FData = NULL;
}

FASTCALL CdlNode::~CdlNode()
{
	Unlink();
}

void FASTCALL CdlNode::LinkPrev( CdlNode *new_node )
{
	new_node->FPrev = FPrev;
	new_node->FNext = this;
	if ( FPrev != NULL )
		FPrev->FNext = new_node;
	FPrev = new_node;
}

void FASTCALL CdlNode::LinkNext( CdlNode *new_node )
{
	new_node->FNext = FNext;
	new_node->FPrev = this;
	if ( FNext != NULL )
		FNext->FPrev = new_node;
	FNext = new_node;
}

void FASTCALL CdlNode::Unlink( void )
{
	if ( FPrev != NULL )
		FPrev->FNext = FNext;
	if ( FNext != NULL )
		FNext->FPrev = FPrev;
	FPrev = NULL;
	FNext = NULL;
}

CdlNode * FASTCALL CdlNode::Delete( void )
{
	CdlNode *node = FPrev;

	if ( node == NULL )
		node = FNext;
	delete this;
	return node;
}

CdlNode * FASTCALL CdlNode::MoveUpBy( long cnt )
{
	CdlNode *node = this;

	while ( cnt > 0 && node->FNext != NULL )
	{
		node = node->FNext;
		cnt--;
	}
	return node;
}

void FASTCALL CdlNode::Sort( long lo, long hi, CdlNode *nlo, CdlNode *nhi, TdlCustomCompareFunc func )
{
	long		i, j;
	CdlNode		*ni, *nj, *pivot;
	void		*data;

	while ( hi > lo )
	{
		i = lo;   ni = nlo;
		j = hi;   nj = nhi;
		pivot = nlo->MoveUpBy( (hi-lo) / 2 );
		data = pivot->FData;
		pivot->FData = ni->FData;
		ni->FData = data;
		while ( i < j )
		{
			while ( i < j && func( nj->FData, data ) > 0 )
			{
				j--;
				nj = nj->FPrev;
			}
			ni->FData = nj->FData;
			while ( i < j && func( ni->FData, data ) <= 0 )
			{
				i++;
				ni = ni->FNext;
			}
			nj->FData = ni->FData;
		}
		ni->FData = data;
		if ( i - lo < hi - i )
		{
			Sort( lo, i-1, nlo, ni->FPrev, func );
			lo = i + 1;
			nlo = ni->FNext;
		}
		else
		{
			Sort( i+1, hi, ni->FNext, nhi, func );
			hi = i - 1;
			nhi = ni->FPrev;
		}
	}
}
//---------------------------------------------------------------------------
FASTCALL CCustomDLList::CCustomDLList( bool can_delete )
{
	FFirst = NULL;
	FLast = NULL;
	FCount = 0;
	FIteratorCurrent = NULL;
	FCanDelete = can_delete;
}

FASTCALL CCustomDLList::~CCustomDLList()
{
	Clear();
}

CdlNode * FASTCALL CCustomDLList::GetNode( long index )
{
	CdlNode		*node;

	if ( index < 0 || index >= FCount )
		ThrowException( TEXT("Invalid TdlList index") );
	if ( FCount / 2 < index )
	{
		index = FCount - index - 1;
		node = FLast;
		while ( index > 0 && node != NULL )
		{
			node = node->FPrev;
			index--;
		}
	}
	else
	{
		node = FFirst;
		while ( index > 0 && node != NULL )
		{
			node = node->FNext;
			index--;
		}
	}
	return node;
}

void * FASTCALL CCustomDLList::Add( void *a_object )
{
	return InsertAtPos( a_object, FCount );
}

void * FASTCALL CCustomDLList::InsertAtPos( void *a_object, long pos )
{
	CdlNode		*new_node;

	try
	{
		new_node = new CdlNode;
		LinkNode( pos, new_node );
	}
	catch( ... )
	{
		if ( FCanDelete )
			DeleteItem( a_object );
		throw;
	}
	new_node->FData = a_object;
	return a_object;
}

void FASTCALL CCustomDLList::LinkNode( long pos, CdlNode *new_node )
{
	if ( pos < 0 || pos > FCount )
		ThrowException( TEXT("Invalid TdlList index") );
	if ( FCount == 0 )
	{
		FFirst = new_node;
		FLast = new_node;
	}
	else if ( pos == 0 )
	{
		FFirst->LinkPrev( new_node );
		FFirst = new_node;
	}
	else if ( pos == FCount )
	{
		FLast->LinkNext( new_node );
		FLast = new_node;
	}
	else
		Nodes[pos]->LinkPrev( new_node );
	FCount++;
}

long FASTCALL CCustomDLList::PosFromObject( void *a_object )
{
	CdlNode		*node = FFirst;
	long		ret_val = 0;

	while ( node != NULL )
	{
		if ( node->FData == a_object )
			break;
		ret_val++;
		node = node->FNext;
	}
	if ( ret_val == 0 )
		ThrowException( TEXT("No Such Object in TdlList") );
	return ret_val;
}

void FASTCALL CCustomDLList::DeleteAtPos( long pos )
{
	CdlNode		*node;

	if ( FCount > 0 )
	{
		node = Nodes[pos];
		UnlinkNode( node );
		if ( FCanDelete )
			DeleteItem( node->FData );
		node->Delete();
	}
}

void FASTCALL CCustomDLList::UnlinkNode( CdlNode *node )
{
	if ( FFirst == node )
		FFirst = node->FNext;
	if ( FLast == node )
		FLast = node->FPrev;
	node->Unlink();
}

void FASTCALL CCustomDLList::Clear( void )
{
	CdlNode		*node = FLast;

	while ( node != NULL )
	{
		if ( FCanDelete )
			DeleteItem( node->FData );
		node = node->Delete();
	}
	FCount = 0;
	FFirst = NULL;
	FLast = NULL;
}

void * FASTCALL CCustomDLList::operator []( long index )
{
	return GetNode( index )->FData;
}

void FASTCALL CCustomDLList::Swap( long pos1, long pos2 )
{
	void		*data;
	CdlNode		*node1 = Nodes[pos1], *node2 = Nodes[pos2];

	data = node1->FData;
	node1->FData = node2->FData;
	node2->FData = data;
}

void FASTCALL CCustomDLList::Move( long from_pos, long to_pos )
{
	CdlNode		*from_node;

	if ( to_pos >= FCount )
		to_pos = FCount - 1;
	if ( from_pos != to_pos )
	{
		from_node = Nodes[from_pos];
		UnlinkNode( from_node );
		LinkNode( to_pos, from_node );
	}
}

void FASTCALL CCustomDLList::QSort( TdlCustomCompareFunc func )
{
	if ( FCount > 1 )
		FFirst->Sort( 0, FCount-1, FFirst, FLast, func );
}

void * FASTCALL CCustomDLList::IteratorFirst( void )
{
	if ( (FIteratorCurrent = FFirst) != NULL )
		return FIteratorCurrent->FData;
	return NULL;
}

void * FASTCALL CCustomDLList::IteratorLast( void )
{
	if ( (FIteratorCurrent = FLast) != NULL )
		return FIteratorCurrent->FData;
	return NULL;
}

void * FASTCALL CCustomDLList::IteratorNext( void )
{
	if ( FIteratorCurrent != NULL )
		if ( (FIteratorCurrent = FIteratorCurrent->FNext) != NULL )
			return FIteratorCurrent->FData;
	return NULL;
}

void * FASTCALL CCustomDLList::IteratorPrev( void )
{
	if ( FIteratorCurrent != NULL )
		if ( (FIteratorCurrent = FIteratorCurrent->FPrev) != NULL )
			return FIteratorCurrent->FData;
	return NULL;
}
//---------------------------------------------------------------------------

