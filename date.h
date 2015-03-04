
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


#ifndef LIBTIME_DATE_H
#define LIBTIME_DATE_H


#include <time.h>

#include <libtypes/types.h>


typedef struct date {
    int year;           // absolute
    uchar month;        // 1 - 12
    uchar day;          // 1 - 31
} Date;


// TODO: add month <-> day correlations
#define DATE_INVARIANTS( d ) \
    ( 1 <= ( d ).month ), ( ( d ).month <= 12 ), \
    ( 1 <= ( d ).day ), ( ( d ).day <= 31 )


bool date__is_valid( Date );


Date date__from_tm( struct tm );

Date date__local_from_time( time_t );
Date date__local_from_timespec( struct timespec );

ord date__compare( Date, Date );

bool date__less_than( Date, Date );
bool date__less_than_or_eq( Date, Date );

bool date__equal( Date, Date );
bool date__not_equal( Date, Date );

bool date__greater_than_or_eq( Date, Date );
bool date__greater_than( Date, Date );


#endif

