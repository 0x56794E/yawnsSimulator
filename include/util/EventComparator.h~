#ifndef EVENTCOMP_H
#define EVENTCOMP_H

#include "Event.h"

/**
 * Comparator for ASC order
 * i.e., smallest element should be at the head
 * of the queue and removed first.
 */
class EventComparator
{
public:
    bool operator() (Event* a, Event* b)
    {
        return a->getTimestamp() > b->getTimestamp();
    }
};

#endif
