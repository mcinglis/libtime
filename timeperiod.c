
#include "timeperiod.h"

#include <time.h>

#include <libbase/intmax.h>


time_t
timeperiod__end( TimePeriod const tp )
{
    // TODO: check for overflows from intmax_t -> time_t (need TIME_MAX)
    return intmax__add_b( tp.start, tp.duration );
}


bool
timeperiod__contains( TimePeriod const tp,
                      time_t const t )
{
    return tp.start <= t && t <= timeperiod__end( tp );
}

