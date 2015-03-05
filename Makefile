
##############################
### VARIABLES
##############################

DEPS_DIR ?= ./deps

CPPFLAGS += -I$(DEPS_DIR) -D_POSIX_C_SOURCE=200809L

cflags_std := -std=c11
cflags_warnings := -Wall -Wextra -Wpedantic \
                   -Wcomments -Wformat=2 -Wlogical-op -Wmissing-include-dirs \
                   -Wnested-externs -Wold-style-definition -Wredundant-decls \
                   -Wshadow -Wstrict-prototypes -Wunused-macros -Wvla \
                   -Wwrite-strings \
                   -Wno-override-init -Wno-unused-parameter

CFLAGS ?= $(cflags_std) -g $(cflags_warnings)

PYTHON ?= python

RENDER_JINJA_SCRIPT ?= $(DEPS_DIR)/render-jinja/render_jinja.py
RENDER_JINJA ?= $(PYTHON) $(RENDER_JINJA_SCRIPT)

time_sys_headers := time.h libmacro/bound.h
time_typeclasses := BOUNDED EQ ORD ENUM NUM FROM_STR
time_min_bound := MIN_BOUND( ( time_t ) 0 )
time_max_bound := MAX_BOUND( ( time_t ) 0 )

gen_headers := time.h
gen_sources := time.c

sources := $(gen_sources) $(wildcard *.c)
objects := $(sources:.c=.o)
mkdeps  := $(objects:.o=.dep.mk)


##############################
### BUILDING
##############################

.PHONY: all
all: objects

.PHONY: fast
fast: CPPFLAGS += -DNDEBUG -DNO_ASSERT -DNO_REQUIRE -DNO_DEBUG
fast: CFLAGS = $(cflags_std) -O3 $(cflags_warnings)
fast: all

.PHONY: objects
objects: $(objects)


time.h: $(DEPS_DIR)/libbase/header.h.jinja
	$(RENDER_JINJA) $< "include_guard=LIBTIME_TIME_H" "sys_headers=time.h" "rel_headers=" "extra=" "type=time_t" "macroname=TIME" "funcname=time" "typeclasses=$(time_typeclasses)" -o $@

time.c: $(DEPS_DIR)/libbase/source.c.jinja time.h
	$(RENDER_JINJA) $< "header=time.h" "sys_headers=$(time_sys_headers)" "rel_headers=" "extra=" "type=time_t" "macroname=TIME" "funcname=time" "typeclasses=$(time_typeclasses)" "min_bound=$(time_min_bound)" "max_bound=$(time_max_bound)" "num_type=signed" -o $@


.PHONY: clean
clean:
	rm -rf $(gen_headers) $(gen_sources) $(objects) $(mkdeps)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@

-include $(mkdeps)


