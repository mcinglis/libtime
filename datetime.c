
#include "datetime.h"

#include <limits.h>

#include <libmacro/assert.h>
#include <libmacro/logic.h>     // ALL

#include "tm.h"


bool
datetime__is_valid( DateTime const dt )
{
    return ALL( DATETIME_INVARIANTS( dt ) );
}


DateTime
datetime__from_tm( struct tm const tm )
{ ASSERT( tm.tm_year <= ( INT_MAX - 1900 ),
          0 <= tm.tm_mon,  tm.tm_mon  <= 11,
          0 <= tm.tm_mday, tm.tm_mday <= 31,
          0 <= tm.tm_hour, tm.tm_hour <= 23,
          0 <= tm.tm_min,  tm.tm_min  <= 59,
          0 <= tm.tm_sec,  tm.tm_sec  <= 60 );
    return ( DateTime ){ .year    = tm.tm_year + 1900,
                         .month   = tm.tm_mon + 1,
                         .day     = tm.tm_mday,
                         .hour    = tm.tm_hour,
                         .minutes = tm.tm_min,
                         .seconds = tm.tm_sec };
}


DateTime
datetime__local_from_time_t( time_t const t )
{
    return datetime__from_tm( tm__local_from_time_t( t ) );
}


DateTime
datetime__local_from_timespec( struct timespec const ts )
{
    return datetime__from_tm( tm__local_from_timespec( ts ) );
}


Date
datetime__date( DateTime const dt )
{ ASSERT( datetime__is_valid( dt ) );
    return ( Date ){ .year  = dt.year,
                     .month = dt.month,
                     .day   = dt.day };
}


Time
datetime__time( DateTime const dt )
{ ASSERT( datetime__is_valid( dt ) );
    return ( Time ){ .hour    = dt.hour,
                     .minutes = dt.minutes,
                     .seconds = dt.seconds };
}


