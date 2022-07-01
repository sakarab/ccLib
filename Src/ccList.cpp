//---------------------------------------------------------------------------
#pragma hdrstop

#include "ccList.h"
#include <stdexcept>
//---------------------------------------------------------------------------
namespace cc
{

void CCustomList::OutOfRangeError()
{
    throw std::out_of_range( "Out of range error." );
}

void CCustomList::RangeError()
{
    throw std::range_error( "Range error." );
}

CCustomList::CCustomList( bool can_delete )
    : FList((void **)0), FCount(0), FCapacity(0), FCanDelete(can_delete)

{
}

CCustomList::~CCustomList( void )
{
}

int CCustomList::Add( void *Item )
{
	int 	result = FCount;

	try
	{
		if ( result == FCapacity )
			Grow();
	}
	catch ( ... )
	{
		if ( FCanDelete )
			DeleteItem( Item );
		throw;
	}
	FList[result] = Item;
	FCount++;
	return ( result );
}

void CCustomList::Clear( void )
{
	if ( FCanDelete )
		for ( int n = FCount - 1 ; n >= 0 ; n-- )
			DeleteItem( Get( n ) );
	SetCount( 0 );
	SetCapacity( 0 );
}

void CCustomList::Delete( int Index )
{
	CheckIndex( Index );
	if ( FCanDelete )
		DeleteItem( FList[Index] );
	FCount--;
	if ( Index < FCount )
		std::memmove( &FList[Index], &FList[Index+1], (FCount - Index) * sizeof(void *) );
}

void CCustomList::Exchange( int Index1, int Index2 )
{
	void	*Item;

	CheckIndex( Index1 );
	CheckIndex( Index2 );
	Item = FList[Index1];
	FList[Index1] = FList[Index2];
	FList[Index2] = Item;
}

CCustomList * CCustomList::Expand( void )
{
	if ( FCount == FCapacity )
		Grow();
	return ( this );
}

void * CCustomList::First( void )
{
	return ( Get( 0 ) );
}

void CCustomList::Put( int Index, void *Item )
{
	CheckIndex( Index );
	if ( FCanDelete && FList[Index] != NULL )
		DeleteItem( FList[Index] );
	FList[Index] = Item;
}

void CCustomList::Grow( void )
{
	int		Delta;

	if ( FCapacity > 64 )
		Delta = FCapacity / 4;
	else if ( FCapacity > 8 )
		Delta = 16;
	else
		Delta = 4;
	SetCapacity( FCapacity + Delta );
}

int CCustomList::IndexOf( void *Item )
{
	int		result = 0;

	while ( result < FCount && FList[result] != Item )
		result++;
	if ( result == FCount )
		result = -1;
	return ( result );
}

void CCustomList::Insert( int Index, void *Item )
{
	if ( Index < 0 || Index > FCount )
        OutOfRangeError();
	if ( FCount == FCapacity )
		Grow();
	if ( Index < FCount )
		std::memmove( &FList[Index + 1], &FList[Index], (FCount - Index) * sizeof(void *) );
	FList[Index] = Item;
	FCount++;
}

void * CCustomList::Last( void )
{
	return ( Get( FCount - 1 ) );
}

void CCustomList::Move( int CurIndex, int NewIndex )
{
	void	*Item;

	if ( CurIndex != NewIndex )
	{
		CheckIndex( NewIndex );
		Item = Get( CurIndex );
		Delete( CurIndex );
		Insert( NewIndex, Item );
	}
}

int CCustomList::Remove( void *Item )
{
	int		result = IndexOf( Item );

	if ( result != -1 )
		Delete( result );
	return ( result );
}

void CCustomList::Pack( void )
{
	for ( int i = FCount - 1 ; i >= 0 ; i-- )
		if ( FList[i] == NULL )
			Delete( i );
}

void CCustomList::SetCapacity( int NewCapacity )
{
	if ( NewCapacity < FCount || NewCapacity > MAX_LIST_SIZE )
		RangeError();
	if ( NewCapacity != FCapacity )
	{
		FList = (void **)std::realloc( FList, NewCapacity * sizeof(void *) );
		FCapacity = NewCapacity;
	}
}

void CCustomList::SetCount( int NewCount )
{
	if ( NewCount < 0 || NewCount > MAX_LIST_SIZE )
		RangeError();
	if ( NewCount > FCapacity )
		SetCapacity( NewCount );
	if ( NewCount > FCount )
		std::memset( FList[FCount], 0, (NewCount - FCount) * sizeof(void *) );
	FCount = NewCount;
}

}; // end namespace cc
//---------------------------------------------------------------------------

