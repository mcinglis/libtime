
#include "timespec.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <time.h>

#include <libtypes/types.h>
#include <libmacro/assert.h>
#include <libmacro/compare.h>
#include <libmacro/logic.h>     // ALL
#include <libbase/long.h>
#include <libbase/intmax.h>
#include <libstr/str.h>

#include "time.h"


bool
timespec__is_valid( struct timespec const ts )
{
    return ALL( TIMESPEC_INVARIANTS( ts ) );
}


struct timespec
timespec__get_clock( clockid_t const clock )
{
    errno = 0;
    struct timespec ts = { 0 };
    clock_gettime( clock, &ts );
    return ts;
}


struct timespec
timespec__get_realtime( void )
{
    return timespec__get_clock( CLOCK_REALTIME );
}


struct timespec
timespec__get_monotonic( void )
{
    return timespec__get_clock( CLOCK_MONOTONIC );
}


bool
timespec__can_add( struct timespec const x,
                   struct timespec const y )
{
    ASSERT( timespec__is_valid( x ), timespec__is_valid( y ) );

    long const nsec = long__add_b( x.tv_nsec, y.tv_nsec );
    if ( nsec > TIMESPEC_MAX_NSEC ) {
        return time__can_add( x.tv_sec, 1 )
            && time__can_add( x.tv_sec + 1, y.tv_sec );
    } else {
        return time__can_add( x.tv_sec, y.tv_sec );
    }
}


struct timespec
timespec__add( struct timespec const x,
               struct timespec const y )
{
    ASSERT( timespec__is_valid( x ),
            timespec__is_valid( y ),
            timespec__can_add( x, y ) );

    long const nsec = long__add_b( x.tv_nsec, y.tv_nsec );
    if ( nsec > TIMESPEC_MAX_NSEC ) {
        return ( struct timespec ){ .tv_sec = x.tv_sec + y.tv_sec + 1,
                                    .tv_nsec = nsec - TIMESPEC_MAX_NSEC };
    } else {
        return ( struct timespec ){ .tv_sec = x.tv_sec + y.tv_sec,
                                    .tv_nsec = nsec };
    }
}


bool
timespec__can_sub( struct timespec const x,
                   struct timespec const y )
{
    ASSERT( timespec__is_valid( x ), timespec__is_valid( y ) );

    long const nsec = long__sub_b( x.tv_nsec, y.tv_nsec );
    if ( nsec < 0 ) {
        return time__can_sub( x.tv_sec, 1 )
            && time__can_sub( x.tv_sec - 1, y.tv_sec );
    } else {
        return time__can_sub( x.tv_sec, y.tv_sec );
    }
}


struct timespec
timespec__sub( struct timespec const x,
               struct timespec const y )
{
    ASSERT( timespec__is_valid( x ),
            timespec__is_valid( y ),
            timespec__can_sub( x, y ) );

    long const nsec = long__sub_b( x.tv_nsec, y.tv_nsec );
    if ( nsec < 0 ) {
        return ( struct timespec ){ .tv_sec = x.tv_sec - y.tv_sec - 1,
                                    .tv_nsec = nsec + TIMESPEC_MAX_NSEC + 1 };
    } else {
        return ( struct timespec ){ .tv_sec = x.tv_sec - y.tv_sec,
                                    .tv_nsec = nsec };
    }
}


struct timespec
timespec__from_str( char const * const str )
{
    ASSERT( str != NULL );

    errno = 0;
    if ( str[ 0 ] == '\0' ) {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
    char * end1;
    long const l1 = strtol( str, &end1, 10 );
    if ( errno ) {
        return ( struct timespec ){ 0 };
    }
    if ( str__equal( end1, "" ) || str__equal_i( end1, "s" ) ) {
        return ( struct timespec ){ .tv_sec = l1 };
    } else if ( str__equal_i( end1, "ns" ) ) {
        if ( l1 > TIMESPEC_MAX_NSEC ) {
            errno = ERANGE;
            return ( struct timespec ){ 0 };
        } else {
            return ( struct timespec ){ .tv_nsec = l1 };
        }
    } else if ( end1[ 0 ] != 's' ) {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
    char * end2;
    long const l2 = strtol( end1 + 1, &end2, 10 );
    if ( errno != 0 ) {
        return ( struct timespec ){ 0 };
    } else if ( l2 > TIMESPEC_MAX_NSEC ) {
        errno = ERANGE;
        return ( struct timespec ){ 0 };
    }
    if ( str__equal_i( end2, "ns" ) ) {
        return ( struct timespec ){ .tv_sec = l1, .tv_nsec = l2 };
    } else {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
}


size_t
timespec__to_str( struct timespec const ts,
                  char * const str,
                  size_t const size )
{
    ASSERT( str != NULL );

    int const n = snprintf( str, size - 1, "%lus%ldns",
                            ts.tv_sec, ts.tv_nsec );
    str[ size - 1 ] = '\0';
    if ( n < 0 ) {
        ASSERT( errno != 0 );
        return 0;
    } else if ( ( uint ) n >= size ) {
        errno = ENOBUFS;
    }
    return ( n < 0 ) ? 0 : n;
}


ord
timespec__compare( struct timespec const l,
                   struct timespec const r )
{
    ASSERT( timespec__is_valid( l ), timespec__is_valid( r ) );

    ord const sec = COMPARE( l.tv_sec, r.tv_sec );
    if ( sec != EQ ) {
        return sec;
    }
    return COMPARE( l.tv_nsec, r.tv_nsec );
}


bool
timespec__less_than_or_eq( struct timespec const l,
                           struct timespec const r )
{
    return timespec__compare( l, r ) <= EQ;
}


bool
timespec__equal( struct timespec const l,
                 struct timespec const r )
{
    return timespec__compare( l, r ) == EQ;
}


bool
timespec__not_equal( struct timespec const l,
                     struct timespec const r )
{
    return timespec__compare( l, r ) != EQ;
}


bool
timespec__greater_than_or_eq( struct timespec const l,
                              struct timespec const r )
{
    return timespec__compare( l, r ) >= EQ;
}


bool
timespec__greater_than( struct timespec const l,
                        struct timespec const r )
{
    return timespec__compare( l, r ) == GT;
}


