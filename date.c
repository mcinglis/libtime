
#include "date.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>

#include <libtypes/types.h>
#include <libmacro/assert.h>
#include <libmacro/compare.h>
#include <libmacro/logic.h>     // ALL
#include <libmacro/minmax.h>

#include "tm.h"


bool
date__is_valid( Date const d )
{
    return ALL( DATE_INVARIANTS( d ) );
}


Date
date__from_tm( struct tm const tm )
{
    ASSERT( tm.tm_year <= ( INT_MAX - 1900 ),
            0 <= tm.tm_mon, tm.tm_mon <= 11,
            0 <= tm.tm_mday, tm.tm_mday <= 31 );

    return ( Date ){ .year  = tm.tm_year + 1900,
                     .month = tm.tm_mon + 1,
                     .day   = tm.tm_mday };
}


Date
date__local_from_time( time_t const t )
{
    return date__from_tm( tm__local_from_time( t ) );
}


Date
date__local_from_timespec( struct timespec const ts )
{
    return date__from_tm( tm__local_from_timespec( ts ) );
}


ord
date__compare( Date const l,
               Date const r )
{
    ASSERT( date__is_valid( l ), date__is_valid( r ) );

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


Date
date__from_str(
        char const * const str )
{
    ASSERT( str != NULL );

    uint year = 0;
    uint month = 0;
    uint day = 0;
    int const n = sscanf( str, "%04u-%02u-%02u", &year, &month, &day );
    Date const d = { .year = year, .month = month, .day = day };
    if ( n != 3 || !ALL( DATE_INVARIANTS( d ) ) ) {
        errno = EBADMSG;
        return ( Date ){ 0 };
    }
    return d;
}


size_t
date__into_str(
        Date const date,
        char * const str,
        size_t const size )
{
    ASSERT( date__is_valid( date ), str != NULL );

    if ( date.year < 0 || date.year > 9999 ) {
        errno = EINVAL;
        return 0;
    }
    int const n = snprintf( str, size - 1, "%04d-%02u-%02u",
                            date.year, date.month, date.day );
    str[ size - 1 ] = '\0';
    if ( n < 0 ) {
        ASSERT( errno != 0 );
        return 0;
    } else if ( ( uint ) n >= size ) {
        errno = ENOBUFS;
    }
    return MAX( n, 0 );
}

