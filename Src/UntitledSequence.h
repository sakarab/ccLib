#ifndef UNTITLED_SEQUENCE_H
#define UNTITLED_SEQUENCE_H

#include <QObject>
#include <memory>
#include <set>
#include <loki/Singleton.h>

/********************************************************************
********    UntitledSequence
********************************************************************/
class UntitledSequence
{
private:
    typedef std::set<int>       container;
private:
    friend struct Loki::CreateUsingNew<UntitledSequence>;
private:
    container       mSequence;
    // singleton
    UntitledSequence();
    ~UntitledSequence();
    UntitledSequence( const UntitledSequence& ) = delete;
    UntitledSequence& operator=(const UntitledSequence&) = delete;
public:
    int getAvailable();
    void Release( int seq );
};

// Instantiate it with a typedef like the following in a cpp file
// typedef Loki::SingletonHolder<UntitledSequence>     TUntitledSequence;

#endif