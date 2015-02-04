
#include "time-period.h"

#include <libtypes/types.h>
#include <libmacro/logic.h>     // ALL
#include <libmacro/require.h>

#include "time.h"


bool
time_period__is_valid( TimePeriod const tp )
{
    return ALL( TIME_PERIOD_INVARIANTS( tp ) );
}


Time
time_period__end( TimePeriod const tp )
{
    REQUIRE( time_period__is_valid( tp ) );

    return time__add( tp.start, time__from_seconds( tp.duration ) );
}


bool
time_period__contains( TimePeriod const tp,
                       Time const t )
{
    REQUIRE( time_period__is_valid( tp ), time__is_valid( t ) );

    return time__less_than_or_eq( tp.start, t )
        && time__less_than_or_eq( t, time_period__end( tp ) );
}

