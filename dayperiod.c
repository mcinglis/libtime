
#include "dayperiod.h"

#include <libmacro/bound.h>
#include <libmacro/minmax.h>
#include <libbase/time.h>

#include "daytime.h"


DayTime
dayperiod__end(
        DayPeriod const dp )
{
    time_t const duration = MIN( dp.duration, MAX_BOUND( ( time_t ) 0 ) );
    return daytime__add( dp.start, daytime__from_seconds( duration ) );
}


bool
dayperiod__contains(
        DayPeriod const dp,
        DayTime const t )
{
    return daytime__less_than_or_eq( dp.start, t )
        && daytime__less_than_or_eq( t, dayperiod__end( dp ) );
}

