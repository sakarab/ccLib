#include <pre_cc.h>
#include "UntitledSequence.h"
#include <algorithm>

/********************************************************************
********    UntitledSequence
********************************************************************/
UntitledSequence::UntitledSequence()
    : mSequence()
{
}

UntitledSequence::~UntitledSequence()
{
}

int UntitledSequence::getAvailable()
{
    // adjacent diff greater then one (1)
    auto    it = std::adjacent_find( mSequence.begin(), mSequence.end(), []( int arg1, int arg2 ) { return arg2 - arg1 > 1; } );
    int     result = (it != mSequence.end()) ? *it : mSequence.size();

    ++result;
    mSequence.insert( result );
    return result;
}

void UntitledSequence::Release( int seq )
{
    mSequence.erase( seq );
}
