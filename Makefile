
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

sources := $(wildcard *.c)
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

.PHONY: clean
clean:
	rm -rf $(objects) $(mkdeps)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@

-include $(mkdeps)


