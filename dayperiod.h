
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


#ifndef LIBTIME_DAYPERIOD_H
#define LIBTIME_DAYPERIOD_H


#include <time.h>

#include <libtypes/types.h>

#include "daytime.h"


typedef struct dayperiod {
    ulong duration;
    DayTime start;
} DayPeriod;


DayTime
dayperiod__end(
        DayPeriod );


bool
dayperiod__contains(
        DayPeriod,
        DayTime );


#endif // ifndef LIBTIME_DAYPERIOD_H

