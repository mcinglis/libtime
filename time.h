
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


#ifndef LIBTIME_TIME_H
#define LIBTIME_TIME_H


#include <time.h>

#include <libtypes/types.h>


typedef struct time {
    uchar hour;         // 0 - 23
    uchar minutes;      // 0 - 59
    uchar seconds;      // 0 - 60 (for leap seconds)
} Time;


#define TIME_INVARIANTS( t ) \
    ( ( t ).hour <= 23 ), \
    ( ( t ).minutes <= 59 ), \
    ( ( t ).seconds <= 60 )

bool time__is_valid( Time );


Time time__from_tm( struct tm );
Time time__from_seconds( time_t );

Time time__local_from_time_t( time_t );
Time time__local_from_timespec( struct timespec );

time_t time__to_seconds( Time );


ord time__compare( Time, Time );

bool time__less_than( Time, Time );
bool time__less_than_or_eq( Time, Time );

bool time__equal( Time, Time );
bool time__not_equal( Time, Time );

bool time__greater_than_or_eq( Time, Time );
bool time__greater_than( Time, Time );


Time time__add( Time, Time );


#endif

