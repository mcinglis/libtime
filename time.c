
#include "time.h"

#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include <libtypes/types.h>
#include <libmacro/assert.h>
#include <libmacro/compare.h>
#include <libmacro/logic.h>     // ALL
#include <libmacro/minmax.h>    // MIN

#include "tm.h"


bool
time__is_valid( Time const t )
{
    return ALL( TIME_INVARIANTS( t ) );
}


Time
time__from_tm( struct tm const tm )
{
    return ( Time ){ .hour    = tm.tm_hour,
                     .minutes = tm.tm_min,
                     .seconds = tm.tm_sec };
}


Time
time__local_from_time_t( time_t const t )
{
    return time__from_tm( tm__local_from_time_t( t ) );
}


Time
time__local_from_timespec( struct timespec const ts )
{
    return time__from_tm( tm__local_from_timespec( ts ) );
}



Time
time__from_seconds( time_t s )
{
    if ( s <= 0 ) {
        return ( Time ){ 0, 0, 0 };
    }
    // TODO: check that arithmetic will have well-defined behavior
    uchar const seconds = s % 60;
    s -= seconds;
    uchar const minutes = ( s % 3600 ) / 60;
    s -= minutes * 60;
    uchar const hour = s / 3600;
    return ( hour > 23 ) ? ( Time ){ 23, 59, 59 }
                         : ( Time ){ hour, minutes, seconds };
}


time_t
time__to_seconds( Time const t )
{
    ASSERT( time__is_valid( t ) );

    return t.seconds + ( t.minutes * 60 ) + ( t.hour * 60 * 60 );
}


ord
time__compare( Time const l,
               Time const r )
{
    ASSERT( time__is_valid( l ), time__is_valid( r ) );

    ord const hour = COMPARE( l.hour, r.hour );
    if ( hour != EQ ) {
        return hour;
    }
    ord const minutes = COMPARE( l.minutes, r.minutes );
    if ( minutes != EQ ) {
        return minutes;
    }
    return COMPARE( l.seconds, r.seconds );
}


bool
time__less_than( Time const l,
                 Time const r )
{
    return time__compare( l, r ) == LT;
}


bool
time__less_than_or_eq( Time const l,
                       Time const r )
{
    return time__compare( l, r ) <= EQ;
}


bool
time__equal( Time const l,
             Time const r )
{
    return time__compare( l, r ) == EQ;
}


bool
time__not_equal( Time const l,
                 Time const r )
{
    return time__compare( l, r ) != EQ;
}


bool
time__greater_than_or_eq( Time const l,
                          Time const r )
{
    return time__compare( l, r ) >= EQ;
}


bool
time__greater_than( Time const l,
                    Time const r )
{
    return time__compare( l, r ) == GT;
}


Time
time__add( Time const l,
           Time const r )
{
    ASSERT( time__is_valid( l ), time__is_valid( r ) );

    return time__from_seconds( time__to_seconds( l ) + time__to_seconds( r ) );
}


Time
time__sub( Time const l,
           Time const r )
{
    ASSERT( time__is_valid( l ), time__is_valid( r ) );

    return time__from_seconds( time__to_seconds( l ) - time__to_seconds( r ) );
}


Time
time__from_str( char const * const str )
{
    ASSERT( str != NULL );

    errno = 0;
    if ( str[ 0 ] == '\0' ) {
        errno = EBADMSG;
        return ( Time ){ 0 };
    }
    char * end1;
    long const hour = strtol( str, &end1, 10 );
    if ( errno ) {
        return ( Time ){ 0 };
    } else if ( end1[ 0 ] != ':' ) {
        errno = EBADMSG;
        return ( Time ){ 0 };
    }
    char * end2;
    long const minutes = strtol( end1 + 1, &end2, 10 );
    if ( errno ) {
        return ( Time ){ 0 };
    }
    long seconds = 0;
    if ( end2[ 0 ] == ':' ) {
        char * end3;
        seconds = strtol( end2 + 1, &end3, 10 );
        if ( errno ) {
            return ( Time ){ 0 };
        } else if ( end3[ 0 ] != '\0' ) {
            errno = EBADMSG;
            return ( Time ){ 0 };
        }
    } else if ( end2[ 0 ] != '\0' ) {
        errno = EBADMSG;
        return ( Time ){ 0 };
    }
    Time const t = { .hour = hour, .minutes = minutes, .seconds = seconds };
    if ( !time__is_valid( t ) ) {
        errno = ERANGE;
        return ( Time ){ 0 };
    }
    return t;
}

