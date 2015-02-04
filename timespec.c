
#include "timespec.h"

#include <stdlib.h>
#include <errno.h>

#include <time.h>

#include <libtypes/types.h>
#include <libmacro/logic.h>     // ALL
#include <libmacro/require.h>
#include <libbase/long.h>
#include <libbase/str.h>


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
timespec__can_add( struct timespec const l,
                   struct timespec const r )
{
    REQUIRE( timespec__is_valid( l ), timespec__is_valid( r ) );

    return long__can_add( l.tv_sec, r.tv_sec )
        && long__can_add( l.tv_nsec, r.tv_nsec );
}


struct timespec
timespec__add( struct timespec const l,
               struct timespec const r )
{
    REQUIRE( timespec__is_valid( l ), timespec__is_valid( r ) );

    time_t const sec = l.tv_sec + r.tv_sec;
    long const nsec = l.tv_nsec + r.tv_nsec;
    if ( nsec > TIMESPEC_MAX_NSEC ) {
        return ( struct timespec ){ .tv_sec = sec + 1,
                                    .tv_nsec = nsec - TIMESPEC_MAX_NSEC };
    } else {
        return ( struct timespec ){ .tv_sec = sec,
                                    .tv_nsec = nsec };
    }
}


struct timespec
timespec__from_str( char const * const str )
{
    REQUIRE( str != NULL );

    errno = 0;
    if ( str[ 0 ] == '\0' ) {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
    char * end1;
    long const l1 = strtol( str, &end1, 10 );
    if ( errno != 0 ) {
        return ( struct timespec ){ 0 };
    }
    if ( str__equal_i( end1, "s" ) ) {
        return ( struct timespec ){ .tv_sec = l1 };
    } else if ( str__equal_i( end1, "ns" ) ) {
        return ( struct timespec ){ .tv_nsec = l1 };
    } else if ( end1[ 0 ] != 's' ) {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
    char * end2;
    long const l2 = strtol( end1 + 1, &end2, 10 );
    if ( errno != 0 ) {
        return ( struct timespec ){ 0 };
    }
    if ( end2[ 0 ] == '\0' || str__equal_i( end2, "ns" ) ) {
        return ( struct timespec ){ .tv_sec = l1, .tv_nsec = l2 };
    } else {
        errno = EBADMSG;
        return ( struct timespec ){ 0 };
    }
}


