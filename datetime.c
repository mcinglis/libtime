
#include "datetime.h"

#include <libmacro/logic.h>     // ALL
#include <libmacro/require.h>


bool
datetime__is_valid( DateTime const dt )
{
    return ALL( DATETIME_INVARIANTS( dt ) );
}


DateTime
datetime__from_tm( struct tm const tm )
{
    DateTime const dt = { .year    = tm.tm_year + 1900,
                          .month   = tm.tm_mon + 1,
                          .day     = tm.tm_mday,
                          .hour    = tm.tm_hour,
                          .minutes = tm.tm_min,
                          .seconds = tm.tm_sec };
    REQUIRE( datetime__is_valid( dt ) );
    return dt;
}


Date
datetime__date( DateTime const dt )
{
    REQUIRE( datetime__is_valid( dt ) );

    return ( Date ){ .year  = dt.year,
                     .month = dt.month,
                     .day   = dt.day };
}


Time
datetime__time( DateTime const dt )
{
    REQUIRE( datetime__is_valid( dt ) );

    return ( Time ){ .hour    = dt.hour,
                     .minutes = dt.minutes,
                     .seconds = dt.seconds };
}


