
#include "date.h"

#include <limits.h>

#include <libtypes/types.h>
#include <libmacro/assert.h>
#include <libmacro/compare.h>
#include <libmacro/logic.h>     // ALL

#include "tm.h"


bool
date__is_valid( Date const d )
{
    return ALL( DATE_INVARIANTS( d ) );
}


Date
date__from_tm( struct tm const tm )
{ ASSERT( tm.tm_year <= ( INT_MAX - 1900 ),
          0 <= tm.tm_mon, tm.tm_mon <= 11,
          0 <= tm.tm_mday, tm.tm_mday <= 31 );
    return ( Date ){ .year  = tm.tm_year + 1900,
                     .month = tm.tm_mon + 1,
                     .day   = tm.tm_mday };
}


Date
date__local_from_time_t( time_t const t )
{
    return date__from_tm( tm__local_from_time_t( t ) );
}


Date
date__local_from_timespec( struct timespec const ts )
{
    return date__from_tm( tm__local_from_timespec( ts ) );
}


ord
date__compare( Date const l,
               Date const r )
{ ASSERT( date__is_valid( l ), date__is_valid( r ) );
    ord const year = COMPARE( l.year, r.year );
    if ( year != EQ ) {
        return year;
    }
    ord const month = COMPARE( l.month, r.month );
    if ( month != EQ ) {
        return month;
    }
    return COMPARE( l.day, r.day );
}


bool
date__less_than( Date const l,
                 Date const r )
{
    return date__compare( l, r ) == LT;
}


bool
date__less_than_or_eq( Date const l,
                       Date const r )
{
    return date__compare( l, r ) <= EQ;
}


bool
date__equal( Date const l,
             Date const r )
{
    return date__compare( l, r ) == EQ;
}


bool
date__not_equal( Date const l,
                 Date const r )
{
    return date__compare( l, r ) != EQ;
}


bool
date__greater_than_or_eq( Date const l,
                          Date const r )
{
    return date__compare( l, r ) >= EQ;
}


bool
date__greater_than( Date const l,
                    Date const r )
{
    return date__compare( l, r ) == GT;
}

