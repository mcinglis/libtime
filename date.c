
#include "date.h"

#include <libtypes/types.h>
#include <libmacro/compare.h>
#include <libmacro/logic.h>     // ALL
#include <libmacro/require.h>


bool
date__is_valid( Date const d )
{
    return ALL( DATE_INVARIANTS( d ) );
}


Date
date__from_tm( struct tm const tm )
{
    Date const d = { .year  = tm.tm_year + 1900,
                     .month = tm.tm_mon + 1,
                     .day   = tm.tm_mday };
    REQUIRE( date__is_valid( d ) );
    return d;
}


ord
date__compare( Date const l,
               Date const r )
{
    REQUIRE( date__is_valid( l ), date__is_valid( r ) );

    ord const year = COMPARE( l.year, r.year );
    if ( year != EQ ) {
        return year;
    }
    ord const month = COMPARE( l.month, r.month );
    if ( month != EQ ) {
        return month;
    }
    return COMPARE( l.day, r.day );
}


bool
date__less_than( Date const l,
                 Date const r )
{
    return date__compare( l, r ) == LT;
}


bool
date__less_than_or_equal( Date const l,
                          Date const r )
{
    return date__compare( l, r ) <= EQ;
}


bool
date__equal( Date const l,
             Date const r )
{
    return date__compare( l, r ) == EQ;
}


bool
date__not_equal( Date const l,
                 Date const r )
{
    return date__compare( l, r ) != EQ;
}


bool
date__greater_than_or_equal( Date const l,
                             Date const r )
{
    return date__compare( l, r ) >= EQ;
}


bool
date__greater_than( Date const l,
                    Date const r )
{
    return date__compare( l, r ) == GT;
}

