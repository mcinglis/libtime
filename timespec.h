
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


#ifndef LIBTIME_TIMESPEC_H
#define LIBTIME_TIMESPEC_H


#include <time.h>

#include <libtypes/types.h>


#define TIMESPEC_MAX_NSEC 999999999L


#define TIMESPEC_INVARIANTS( ts ) \
    ( 0 <= ( ts ).tv_nsec ), ( ( ts ).tv_nsec <= TIMESPEC_MAX_NSEC )


bool
timespec__is_valid( struct timespec );


struct timespec
timespec__get_clock( clockid_t );


struct timespec
timespec__get_realtime( void );


struct timespec
timespec__get_monotonic( void );


bool
timespec__can_add( struct timespec,
                   struct timespec );


struct timespec
timespec__add( struct timespec,
               struct timespec );


struct timespec
timespec__from_str( char const * str );


#endif


