# what flags you want to pass to the C compiler & linker
#CFLAGS = --pedantic -Wall -std=c99
#LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in the previous manual Makefile

# the library names to build (note we are building static libs only)
lib_LIBRARIES = libe4c.a

# the list of header files that belong to the library (to be installed later)
include_HEADERS = e4c.h

# the sources to add to the library and to add to the source distribution
libe4c_a_SOURCES =  $(include_HEADERS) e4c.c
