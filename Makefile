
##############################
### VARIABLES
##############################

DEPS_DIR ?= ./deps

LIBBASE  ?= $(DEPS_DIR)/libbase
LIBSTR   ?= $(DEPS_DIR)/libstr

CPPFLAGS += -I$(DEPS_DIR) -D_POSIX_C_SOURCE=200809L

cflags_std := -std=c11
cflags_warnings := -Wall -Wextra -Wpedantic \
                   -Wcomments -Wformat=2 -Wlogical-op -Wmissing-include-dirs \
                   -Wnested-externs -Wold-style-definition -Wredundant-decls \
                   -Wshadow -Wstrict-prototypes -Wunused-macros -Wvla \
                   -Wwrite-strings \
                   -Wno-override-init -Wno-unused-parameter

CFLAGS ?= $(cflags_std) -g $(cflags_warnings)

TPLRENDER ?= $(DEPS_DIR)/tplrender/tplrender


libbase_types  := long intmax
libtime_types  := time

long_type    := long
long_options := --typeclasses BOUNDED EQ ORD ENUM NUM FROM_STR TO_STRM \
                --extra num_type=signed

intmax_type    := intmax_t
intmax_options := --typeclasses BOUNDED EQ ORD ENUM NUM FROM_STR TO_STRM \
                  --extra num_type=signed

size_type    := size_t
size_options := --typeclasses BOUNDED EQ ORD ENUM NUM FROM_STR TO_STRM \
                --extra num_type=unsigned

time_type    := time_t
time_options := --typeclasses BOUNDED EQ ORD ENUM NUM FROM_STR TO_STRM \
                --sys-headers time.h libmacro/bound.h \
                --extra num_type=signed \
                        min_bound="MIN_BOUND( ( time_t ) 0 )" \
                        max_bound="MAX_BOUND( ( time_t ) 0 )"

libbase_sources := $(foreach t,$(libbase_types),$(LIBBASE)/$t.c)
libbase_headers := $(libbase_sources:.c=.h)
libbase_objects := $(libbase_sources:.c=.o)

libtime_sources := $(addsuffix .c,$(libtime_types))
libtime_headers := $(libtime_sources:.c=.h)
libtime_objects := $(libtime_sources:.c=.o)

gen_objects := $(libbase_objects) $(libtime_objects)
gen := $(libbase_sources) \
       $(libbase_headers) \
       $(libtime_sources) \
       $(libtime_headers) \
       $(gen_objects)

sources := $(wildcard *.c)
objects := $(sources:.c=.o)
mkdeps  := $(objects:.o=.dep.mk)


##############################
### BUILDING
##############################

.PHONY: all
all: objects

.PHONY: fast
fast: CPPFLAGS += -DNDEBUG
fast: CFLAGS = $(cflags_std) -O3 $(cflags_warnings)
fast: all

.PHONY: objects
objects: $(objects) $(gen_objects)

.PHONY: clean
clean:
	rm -rf $(objects) $(gen) $(mkdeps)


%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@


timeperiod.o: $(LIBBASE)/intmax.h

timespec.o: $(LIBBASE)/long.h \
            time.h

name_from_path = $(subst -,_,$1)

$(libbase_headers): $(LIBBASE)/%.h: $(LIBBASE)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libbase_sources): $(LIBBASE)/%.c: $(LIBBASE)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libbase_objects): $(LIBBASE)/%.o: $(LIBBASE)/%.h

$(libtime_headers): $(LIBBASE)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libtime_sources): $(LIBBASE)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libtime_objects): %.o: %.h


-include $(mkdeps)

