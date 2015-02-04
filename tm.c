

#include "tm.h"

#include <errno.h>

#include <time.h>


struct tm
tm__local_from_time_t( time_t const t )
{
    errno = 0;
    struct tm tm = { 0 };
    localtime_r( &t, &tm );
    return tm;
}


struct tm
tm__local_from_timespec( struct timespec const ts )
{
    return tm__local_from_time_t( ts.tv_sec );
}


