

#include "tm.h"

#include <errno.h>

#include <time.h>


struct tm
tm__local_from_seconds( time_t const seconds )
{
    errno = 0;
    struct tm tm = { 0 };
    localtime_r( &seconds, &tm );
    return tm;
}


struct tm
tm__local_from_timespec( struct timespec const ts )
{
    return tm__local_from_seconds( ts.tv_sec );
}


