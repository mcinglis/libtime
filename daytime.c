
#include "daytime.h"

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
daytime__is_valid( DayTime const t )
{
    return ALL( DAYTIME_INVARIANTS( t ) );
}


DayTime
daytime__from_tm( struct tm const tm )
{
    return ( DayTime ){ .hour    = tm.tm_hour,
                        .minutes = tm.tm_min,
                        .seconds = tm.tm_sec };
}


DayTime
daytime__local_from_time( time_t const t )
{
    return daytime__from_tm( tm__local_from_time( t ) );
}


DayTime
daytime__local_from_timespec( struct timespec const ts )
{
    return daytime__from_tm( tm__local_from_timespec( ts ) );
}


DayTime
daytime__from_seconds( time_t s )
{
    if ( s <= 0 ) {
        return ( DayTime ){ 0, 0, 0 };
    }
    // TODO: check that arithmetic will have well-defined behavior
    uchar const seconds = s % 60;
    s -= seconds;
    uchar const minutes = ( s % 3600 ) / 60;
    s -= minutes * 60;
    uchar const hour = s / 3600;
    return ( hour > 23 ) ? ( DayTime ){ 23, 59, 59 }
                         : ( DayTime ){ hour, minutes, seconds };
}


time_t
daytime__to_seconds( DayTime const t )
{
    ASSERT( daytime__is_valid( t ) );

    return t.seconds + ( t.minutes * 60 ) + ( t.hour * 60 * 60 );
}


ord
daytime__compare( DayTime const l,
               DayTime const r )
{
    ASSERT( daytime__is_valid( l ), daytime__is_valid( r ) );

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
daytime__less_than( DayTime const l,
                 DayTime const r )
{
    return daytime__compare( l, r ) == LT;
}


bool
daytime__less_than_or_eq( DayTime const l,
                       DayTime const r )
{
    return daytime__compare( l, r ) <= EQ;
}


bool
daytime__equal( DayTime const l,
             DayTime const r )
{
    return daytime__compare( l, r ) == EQ;
}


bool
daytime__not_equal( DayTime const l,
                 DayTime const r )
{
    return daytime__compare( l, r ) != EQ;
}


bool
daytime__greater_than_or_eq( DayTime const l,
                          DayTime const r )
{
    return daytime__compare( l, r ) >= EQ;
}


bool
daytime__greater_than( DayTime const l,
                    DayTime const r )
{
    return daytime__compare( l, r ) == GT;
}


DayTime
daytime__add( DayTime const l,
           DayTime const r )
{
    ASSERT( daytime__is_valid( l ), daytime__is_valid( r ) );

    return daytime__from_seconds(
                   daytime__to_seconds( l ) + daytime__to_seconds( r ) );
}


DayTime
daytime__sub( DayTime const l,
           DayTime const r )
{
    ASSERT( daytime__is_valid( l ), daytime__is_valid( r ) );

    return daytime__from_seconds(
                   daytime__to_seconds( l ) - daytime__to_seconds( r ) );
}


DayTime
daytime__from_str( char const * const str )
{
    ASSERT( str != NULL );

    errno = 0;
    if ( str[ 0 ] == '\0' ) {
        errno = EBADMSG;
        return ( DayTime ){ 0 };
    }
    char * end1;
    long const hour = strtol( str, &end1, 10 );
    if ( errno ) {
        return ( DayTime ){ 0 };
    } else if ( end1[ 0 ] != ':' ) {
        errno = EBADMSG;
        return ( DayTime ){ 0 };
    }
    char * end2;
    long const minutes = strtol( end1 + 1, &end2, 10 );
    if ( errno ) {
        return ( DayTime ){ 0 };
    }
    long seconds = 0;
    if ( end2[ 0 ] == ':' ) {
        char * end3;
        seconds = strtol( end2 + 1, &end3, 10 );
        if ( errno ) {
            return ( DayTime ){ 0 };
        } else if ( end3[ 0 ] != '\0' ) {
            errno = EBADMSG;
            return ( DayTime ){ 0 };
        }
    } else if ( end2[ 0 ] != '\0' ) {
        errno = EBADMSG;
        return ( DayTime ){ 0 };
    }
    DayTime const t = { .hour    = hour,
                        .minutes = minutes,
                        .seconds = seconds };
    if ( !daytime__is_valid( t ) ) {
        errno = ERANGE;
        return ( DayTime ){ 0 };
    }
    return t;
}


void
daytime__arg_parse( char const * const name,
                    char const * const arg,
                    void * const vdest )
{
    ASSERT( arg != NULL, vdest != NULL );

    DayTime * const dt = vdest;
    *dt = daytime__from_str( arg );
}

