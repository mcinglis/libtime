
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


#ifndef LIBTIME_TIMEPERIOD_H
#define LIBTIME_TIMEPERIOD_H


#include <time.h>

#include <libtypes/types.h>



typedef struct timeperiod {
    time_t start;
    time_t duration;
} TimePeriod;


time_t
timeperiod__end( TimePeriod );


bool
timeperiod__contains( TimePeriod,
                      time_t );


#endif // ifndef LIBTIME_TIMEPERIOD_H

