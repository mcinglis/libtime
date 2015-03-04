
// Copyright 2015  Malcolm Inglis <http://minglis.id.au>
//
// This file is part of Libtime.
//
// Libtime is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// Libtime is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
// more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Libtime. If not, see <https://gnu.org/licenses/>.


#ifndef LIBTIME_DATETIME_H
#define LIBTIME_DATETIME_H


#include <time.h>

#include <libtypes/types.h>

#include "daytime.h"
#include "date.h"


typedef struct datetime {
    int year;           // absolute
    uchar month;        // 1 - 12
    uchar day;          // 1 - 31
    uchar hour;         // 0 - 23
    uchar minutes;      // 0 - 59
    uchar seconds;      // 0 - 60 (for leap seconds)
} DateTime;


// TODO: add month <-> day correlations
#define DATETIME_INVARIANTS( dt ) \
    ( 1 <= ( dt ).month ), ( ( dt ).month <= 12 ), \
    ( 1 <= ( dt ).day ), ( ( dt ).day <= 31 ), \
    ( ( dt ).hour <= 23 ), \
    ( ( dt ).minutes <= 59 ), \
    ( ( dt ).seconds <= 60 )


bool datetime__is_valid( DateTime );


DateTime datetime__from_tm( struct tm );

DateTime datetime__local_from_time( time_t );
DateTime datetime__local_from_timespec( struct timespec );

Date datetime__date( DateTime );
DayTime datetime__daytime( DateTime );


#endif



