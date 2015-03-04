
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


#ifndef LIBTIME_DAYTIME_H
#define LIBTIME_DAYTIME_H


#include <time.h>

#include <libtypes/types.h>


typedef struct daytime {
    uchar hour;         // 0 - 23
    uchar minutes;      // 0 - 59
    uchar seconds;      // 0 - 60 (for leap seconds)
} DayTime;


#define DAYTIME_INVARIANTS( t ) \
    ( ( t ).hour <= 23 ), \
    ( ( t ).minutes <= 59 ), \
    ( ( t ).seconds <= 60 )

bool daytime__is_valid( DayTime );


DayTime daytime__from_tm( struct tm );
DayTime daytime__from_seconds( time_t );

DayTime daytime__local_from_time( time_t );
DayTime daytime__local_from_timespec( struct timespec );

time_t daytime__to_seconds( DayTime );


ord daytime__compare( DayTime, DayTime );

bool daytime__less_than( DayTime, DayTime );
bool daytime__less_than_or_eq( DayTime, DayTime );

bool daytime__equal( DayTime, DayTime );
bool daytime__not_equal( DayTime, DayTime );

bool daytime__greater_than_or_eq( DayTime, DayTime );
bool daytime__greater_than( DayTime, DayTime );


DayTime daytime__add( DayTime, DayTime );

DayTime daytime__sub( DayTime, DayTime );


DayTime
daytime__from_str( char const * str );


#endif

