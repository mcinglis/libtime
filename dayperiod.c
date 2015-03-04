
#include "dayperiod.h"

#include "daytime.h"


DayTime
dayperiod__end( DayPeriod const dp )
{
    return daytime__add( dp.start, daytime__from_seconds( dp.duration ) );
}


bool
dayperiod__contains( DayPeriod const dp,
                     DayTime const t )
{
    return daytime__less_than_or_eq( dp.start, t )
        && daytime__less_than_or_eq( t, dayperiod__end( dp ) );
}

