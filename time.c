
#include "time.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>

#include <libmacro/bound.h>     // MIN_BOUND, MAX_BOUND


time_t
time__min_bound( void )
{
    return MIN_BOUND( ( time_t ) 0 );
}


time_t
time__max_bound( void )
{
    return MAX_BOUND( ( time_t ) 0 );
}


bool
time__equal( time_t const x,
             time_t const y )
{
    return x == y;
}


bool
time__not_equal( time_t const x,
                 time_t const y )
{
    return x != y;
}


time_t
time__from_str( char const * const str )
{
    if ( str == NULL || str[ 0 ] == '\0' ) {
        errno = EINVAL;
        return 0;
    }
    errno = 0;
    char * end_ptr;

    intmax_t const x = strtoimax( str, &end_ptr, 10 );

    if ( errno ) {
        return 0;
    }
    while ( isspace( *end_ptr ) ) {
        end_ptr++;
    }
    if ( *end_ptr != '\0' ) {
        errno = EBADMSG;
        return 0;
    } else if ( x < time__min_bound() ) {
        errno = ERANGE;
        return time__min_bound();
    } else if ( x > time__max_bound() ) {
        errno = ERANGE;
        return time__max_bound();
    } else {
        return x;
    }
}


void
arg_parse_time( char const * const name,
                char const * const arg,
                void * const vdest )
{
    if ( vdest == NULL ) {
        errno = EINVAL;
    } else {
        time_t * const t = vdest;
        *t = time__from_str( arg );
    }
}

