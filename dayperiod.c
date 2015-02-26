
#include "dayperiod.h"


Time
dayperiod__end( DayPeriod const dp )
{
    return time__add( dp.start, time__from_seconds( dp.duration ) );
}


bool
dayperiod__contains( DayPeriod const dp,
                     Time const t )
{
    return time__less_than_or_eq( dp.start, t )
        && time__less_than_or_eq( t, dayperiod__end( dp ) );
}

