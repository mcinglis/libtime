
##############################
### VARIABLES
##############################

DEPS_DIR ?= deps

LIBBASE   ?= $(DEPS_DIR)/libbase
LIBSTR    ?= $(DEPS_DIR)/libstr
TPLRENDER ?= $(DEPS_DIR)/tplrender/tplrender

CPPFLAGS += -I$(DEPS_DIR) -D_POSIX_C_SOURCE=200809L

cflags_std := -std=c11
cflags_warnings := -Wall -Wextra -Wpedantic \
                   -Wcomments -Wformat=2 -Wlogical-op -Wmissing-include-dirs \
                   -Wnested-externs -Wold-style-definition -Wredundant-decls \
                   -Wshadow -Wstrict-prototypes -Wunused-macros -Wvla \
                   -Wwrite-strings \
                   -Wno-override-init -Wno-unused-parameter

CFLAGS ?= $(cflags_std) -g $(cflags_warnings)


libbase_types  := long intmax time

long_type    := long
long_options := --typeclasses NUM

intmax_type    := intmax_t
intmax_options := --typeclasses NUM

time_type    := time_t
time_options := --typeclasses NUM --sys-headers time.h

libbase_headers := $(foreach t,$(libbase_types),$(LIBBASE)/$t.h)


sources := $(wildcard *.c)
objects := $(sources:.c=.o)
mkdeps  := $(objects:.o=.dep.mk)


##############################
### BUILDING
##############################

.PHONY: all
all: objects

.PHONY: objects
objects: $(objects)

.PHONY: clean
clean:
	rm -rf $(objects) $(libbase_headers) $(mkdeps)


%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@


timeperiod.o: $(LIBBASE)/intmax.h

timespec.o: $(LIBBASE)/long.h \
            $(LIBBASE)/time.h


name_from_path = $(subst -,_,$1)

$(libbase_headers): $(LIBBASE)/%.h: $(LIBBASE)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@


-include $(mkdeps)

