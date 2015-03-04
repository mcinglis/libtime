

#include "tm.h"

#include <errno.h>

#include <time.h>


struct tm
tm__local_from_time( time_t const t )
{
    errno = 0;
    struct tm tm = { 0 };
    localtime_r( &t, &tm );
    return errno ? ( struct tm ){ 0 } : tm;
}


struct tm
tm__local_from_timespec( struct timespec const ts )
{
    return tm__local_from_time( ts.tv_sec );
}


struct tm
tm__get_local( void )
{
    errno = 0;
    time_t const t = time( NULL );
    if ( errno ) {
        return ( struct tm ){ 0 };
    }
    return tm__local_from_time( t );
}


