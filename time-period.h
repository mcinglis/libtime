
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


#ifndef LIBTIME_TIME_PERIOD_H
#define LIBTIME_TIME_PERIOD_H


#include <time.h>

#include <libtypes/types.h>

#include "time.h"



typedef struct time_period {
    Time start;
    time_t duration;
} TimePeriod;


#define TIME_PERIOD_INVARIANTS( tp ) \
    ( time__is_valid( tp.start ) )


bool time_period__is_valid( TimePeriod );


Time time_period__end( TimePeriod );


bool time_period__contains( TimePeriod, Time );


#endif

