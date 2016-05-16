#
# exceptions4c
#
# Copyright (c) 2016 Guillermo Calvo
# Licensed under the GNU Lesser General Public License
#

AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = -Wall -Werror --pedantic -Wno-missing-braces -coverage -O0 -I$(E4C_PATH) -I$(E4C_LITE_PATH)


# Library

E4C_PATH            = src
E4C_LIBRARY         = lib/libe4c.a

E4C_LITE_PATH       = lite/src
E4C_LITE_LIBRARY    = lib/libe4c_lite.a


# Install

lib_LIBRARIES =                         \
    $(E4C_LIBRARY)                      \
    $(E4C_LITE_LIBRARY)

include_HEADERS =                       \
    src/e4c.h                           \
    lite/src/e4c_lite.h


# Documentation

docdir = $(datadir)/doc/exceptions4c
doc_DATA = doc/*


# Cleanup

CLEANFILES =                            \
    *.log                               \
    *.gcda                              \
    *.gcno                              \
    *.gcov                              \
    *.stackdump                         \
    src/*.gcda                          \
    src/*.gcno                          \
    src/*.gcov                          \
    test/*.gcda                         \
    test/*.gcno                         \
    test/*.gcov                         \
    lite/src/*.gcda                     \
    lite/src/*.gcno                     \
    lite/src/*.gcov                     \
    lite/test/*.gcda                    \
    lite/test/*.gcno                    \
    lite/test/*.gcov


# Check

check_PROGRAMS =                        \
	bin/check/a01                       \
	bin/check/a02                       \
	bin/check/a03                       \
	bin/check/a04                       \
	bin/check/a05                       \
	bin/check/a06                       \
	bin/check/a07                       \
	bin/check/a08                       \
	bin/check/a09                       \
	bin/check/a10                       \
	bin/check/a11                       \
	bin/check/a12                       \
	bin/check/a13                       \
	bin/check/a14                       \
	bin/check/a15                       \
	bin/check/a16                       \
	bin/check/b01                       \
	bin/check/b02                       \
	bin/check/b03                       \
	bin/check/b04                       \
	bin/check/b05                       \
	bin/check/b06                       \
	bin/check/b07                       \
	bin/check/b08                       \
	bin/check/b09                       \
	bin/check/b10                       \
	bin/check/b11                       \
	bin/check/b12                       \
	bin/check/b13                       \
	bin/check/b14                       \
	bin/check/b15                       \
	bin/check/b16                       \
	bin/check/b17                       \
	bin/check/b18                       \
	bin/check/b19                       \
	bin/check/b20                       \
	bin/check/b21                       \
	bin/check/b22                       \
	bin/check/c01                       \
	bin/check/c02                       \
	bin/check/d01                       \
	bin/check/d02                       \
	bin/check/d03                       \
	bin/check/d04                       \
	bin/check/d05                       \
	bin/check/e01                       \
	bin/check/e02                       \
	bin/check/e03                       \
	bin/check/f01                       \
	bin/check/f02                       \
	bin/check/f03                       \
	bin/check/f04                       \
	bin/check/f05                       \
	bin/check/f06                       \
	bin/check/f07                       \
	bin/check/g01                       \
	bin/check/g02                       \
	bin/check/g03                       \
	bin/check/g04                       \
	bin/check/g05                       \
	bin/check/g06                       \
	bin/check/g07                       \
	bin/check/g08                       \
	bin/check/g09                       \
	bin/check/g10                       \
	bin/check/g11                       \
	bin/check/g12                       \
	bin/check/g13                       \
	bin/check/g14                       \
	bin/check/g15                       \
	bin/check/g16                       \
	bin/check/g17                       \
	bin/check/g18                       \
	bin/check/g19                       \
	bin/check/g20                       \
	bin/check/g21                       \
	bin/check/g22                       \
	bin/check/h01                       \
	bin/check/h02                       \
	bin/check/h03                       \
	bin/check/h04                       \
	bin/check/h05                       \
	bin/check/h06                       \
	bin/check/h07                       \
	bin/check/h08                       \
	bin/check/h09                       \
	bin/check/h10                       \
	bin/check/h11                       \
	bin/check/i01                       \
	bin/check/i02                       \
	bin/check/i03                       \
	bin/check/i04                       \
	bin/check/i05                       \
	bin/check/i06                       \
	bin/check/i07                       \
	bin/check/i08                       \
	bin/check/i09                       \
	bin/check/i10                       \
	bin/check/i11                       \
	bin/check/i12                       \
	bin/check/i13                       \
	bin/check/i14                       \
	bin/check/i15                       \
	bin/check/i16                       \
	bin/check/i17                       \
    bin/check/lite/uncaught             \
    bin/check/lite/caught               \
    bin/check/lite/cleanup              \
    bin/check/lite/hierarchy            \
    bin/check/lite/limits               \
    bin/check/lite/overflow

TESTS =                                 \
	bin/check/a01                       \
	bin/check/a02                       \
	bin/check/a03                       \
	bin/check/a04                       \
	bin/check/a05                       \
	bin/check/a06                       \
	bin/check/a07                       \
	bin/check/a08                       \
	bin/check/a09                       \
	bin/check/a10                       \
	bin/check/a11                       \
	bin/check/a12                       \
	bin/check/a13                       \
	bin/check/a14                       \
	bin/check/a15                       \
	bin/check/a16                       \
	bin/check/b01                       \
	bin/check/b02                       \
	bin/check/b03                       \
	bin/check/b04                       \
	bin/check/b05                       \
	bin/check/b06                       \
	bin/check/b07                       \
	bin/check/b08                       \
	bin/check/b09                       \
	bin/check/b10                       \
	bin/check/b11                       \
	bin/check/b12                       \
	bin/check/b13                       \
	bin/check/b14                       \
	bin/check/b15                       \
	bin/check/b16                       \
	bin/check/b17                       \
	bin/check/b18                       \
	bin/check/b19                       \
	bin/check/b20                       \
	bin/check/b21                       \
	bin/check/b22                       \
	bin/check/c01                       \
	bin/check/c02                       \
	bin/check/d01                       \
	bin/check/d02                       \
	bin/check/d03                       \
	bin/check/d04                       \
	bin/check/d05                       \
	bin/check/e01                       \
	bin/check/e02                       \
	bin/check/e03                       \
	bin/check/f01                       \
	bin/check/f02                       \
	bin/check/f03                       \
	bin/check/f04                       \
	bin/check/f05                       \
	bin/check/f06                       \
	bin/check/f07                       \
	bin/check/g01                       \
	bin/check/g02                       \
	bin/check/g03                       \
	bin/check/g04                       \
	bin/check/g05                       \
	bin/check/g06                       \
	bin/check/g07                       \
	bin/check/g08                       \
	bin/check/g09                       \
	bin/check/g10                       \
	bin/check/g11                       \
	bin/check/g12                       \
	bin/check/g13                       \
	bin/check/g14                       \
	bin/check/g15                       \
	bin/check/g16                       \
	bin/check/g17                       \
	bin/check/g18                       \
	bin/check/g19                       \
	bin/check/g20                       \
	bin/check/g21                       \
	bin/check/g22                       \
	bin/check/h01                       \
	bin/check/h02                       \
	bin/check/h03                       \
	bin/check/h04                       \
	bin/check/h05                       \
	bin/check/h06                       \
	bin/check/h07                       \
	bin/check/h08                       \
	bin/check/h09                       \
	bin/check/h10                       \
	bin/check/h11                       \
	bin/check/i01                       \
	bin/check/i02                       \
	bin/check/i03                       \
	bin/check/i04                       \
	bin/check/i05                       \
	bin/check/i06                       \
	bin/check/i07                       \
	bin/check/i08                       \
	bin/check/i09                       \
	bin/check/i10                       \
	bin/check/i11                       \
	bin/check/i12                       \
	bin/check/i13                       \
	bin/check/i14                       \
	bin/check/i15                       \
	bin/check/i16                       \
	bin/check/i17                       \
    bin/check/lite/uncaught             \
    bin/check/lite/caught               \
    bin/check/lite/cleanup              \
    bin/check/lite/hierarchy            \
    bin/check/lite/limits               \
    bin/check/lite/overflow

XFAIL_TESTS =                           \
	bin/check/a01                       \
	bin/check/a02                       \
	bin/check/a03                       \
	bin/check/a04                       \
	bin/check/a05                       \
	bin/check/a06                       \
	bin/check/a07                       \
	bin/check/a08                       \
	bin/check/a09                       \
	bin/check/a10                       \
	bin/check/a11                       \
	bin/check/a12                       \
	bin/check/a13                       \
	bin/check/a14                       \
	bin/check/a15                       \
	bin/check/a16                       \
	bin/check/b01                       \
	bin/check/b02                       \
	bin/check/b03                       \
	bin/check/b04                       \
	bin/check/b05                       \
	bin/check/b06                       \
	bin/check/b07                       \
	bin/check/b08                       \
	bin/check/b09                       \
	bin/check/b10                       \
	bin/check/b14                       \
	bin/check/b15                       \
	bin/check/b16                       \
	bin/check/b17                       \
	bin/check/b18                       \
	bin/check/b19                       \
	bin/check/b20                       \
	bin/check/b21                       \
	bin/check/d01                       \
	bin/check/d02                       \
	bin/check/d03                       \
	bin/check/d04                       \
	bin/check/d05                       \
	bin/check/g01                       \
	bin/check/g02                       \
	bin/check/g03                       \
	bin/check/g04                       \
	bin/check/g05                       \
	bin/check/g06                       \
	bin/check/h06                       \
	bin/check/h11                       \
	bin/check/i05                       \
    bin/check/lite/uncaught             \
    bin/check/lite/overflow

tests: check


# exceptions4c library

lib_libe4c_a_CFLAGS                     = -Wall -Werror --pedantic -Wno-missing-braces
lib_libe4c_a_SOURCES                    = src/e4c.c


# exceptions4c library (lightweight version)

lib_libe4c_lite_a_CFLAGS                = -Wall -Werror --pedantic -Wno-missing-braces
lib_libe4c_lite_a_SOURCES               = lite/src/e4c_lite.c


# exceptions4c tests

bin_check_a01_SOURCES                   = src/e4c.c test/a01.c
bin_check_a02_SOURCES                   = src/e4c.c test/a02.c
bin_check_a03_SOURCES                   = src/e4c.c test/a03.c
bin_check_a04_SOURCES                   = src/e4c.c test/a04.c
bin_check_a05_SOURCES                   = src/e4c.c test/a05.c
bin_check_a06_SOURCES                   = src/e4c.c test/a06.c
bin_check_a07_SOURCES                   = src/e4c.c test/a07.c
bin_check_a08_SOURCES                   = src/e4c.c test/a08.c
bin_check_a09_SOURCES                   = src/e4c.c test/a09.c
bin_check_a10_SOURCES                   = src/e4c.c test/a10.c
bin_check_a11_SOURCES                   = src/e4c.c test/a11.c
bin_check_a12_SOURCES                   = src/e4c.c test/a12.c
bin_check_a13_SOURCES                   = src/e4c.c test/a13.c
bin_check_a14_SOURCES                   = src/e4c.c test/a14.c
bin_check_a15_SOURCES                   = src/e4c.c test/a15.c
bin_check_a16_SOURCES                   = src/e4c.c test/a16.c
bin_check_b01_SOURCES                   = src/e4c.c test/b01.c
bin_check_b02_SOURCES                   = src/e4c.c test/b02.c
bin_check_b03_SOURCES                   = src/e4c.c test/b03.c
bin_check_b04_SOURCES                   = src/e4c.c test/b04.c
bin_check_b05_SOURCES                   = src/e4c.c test/b05.c
bin_check_b06_SOURCES                   = src/e4c.c test/b06.c
bin_check_b07_SOURCES                   = src/e4c.c test/b07.c
bin_check_b08_SOURCES                   = src/e4c.c test/b08.c
bin_check_b09_SOURCES                   = src/e4c.c test/b09.c
bin_check_b10_SOURCES                   = src/e4c.c test/b10.c
bin_check_b11_SOURCES                   = src/e4c.c test/b11.c
bin_check_b12_SOURCES                   = src/e4c.c test/b12.c
bin_check_b13_SOURCES                   = src/e4c.c test/b13.c
bin_check_b14_SOURCES                   = src/e4c.c test/b14.c
bin_check_b15_SOURCES                   = src/e4c.c test/b15.c
bin_check_b16_SOURCES                   = src/e4c.c test/b16.c
bin_check_b17_SOURCES                   = src/e4c.c test/b17.c
bin_check_b18_SOURCES                   = src/e4c.c test/b18.c
bin_check_b19_SOURCES                   = src/e4c.c test/b19.c
bin_check_b20_SOURCES                   = src/e4c.c test/b20.c
bin_check_b21_SOURCES                   = src/e4c.c test/b21.c
bin_check_b22_SOURCES                   = src/e4c.c test/b22.c
bin_check_c01_SOURCES                   = src/e4c.c test/c01.c
bin_check_c02_SOURCES                   = src/e4c.c test/c02.c
bin_check_d01_SOURCES                   = src/e4c.c test/d01.c
bin_check_d02_SOURCES                   = src/e4c.c test/d02.c
bin_check_d03_SOURCES                   = src/e4c.c test/d03.c
bin_check_d04_SOURCES                   = src/e4c.c test/d04.c
bin_check_d05_SOURCES                   = src/e4c.c test/d05.c
bin_check_e01_SOURCES                   = src/e4c.c test/e01.c
bin_check_e02_SOURCES                   = src/e4c.c test/e02.c
bin_check_e03_SOURCES                   = src/e4c.c test/e03.c
bin_check_f01_SOURCES                   = src/e4c.c test/f01.c
bin_check_f02_SOURCES                   = src/e4c.c test/f02.c
bin_check_f03_SOURCES                   = src/e4c.c test/f03.c
bin_check_f04_SOURCES                   = src/e4c.c test/f04.c
bin_check_f05_SOURCES                   = src/e4c.c test/f05.c
bin_check_f06_SOURCES                   = src/e4c.c test/f06.c
bin_check_f07_SOURCES                   = src/e4c.c test/f07.c
bin_check_g01_SOURCES                   = src/e4c.c test/g01.c
bin_check_g02_SOURCES                   = src/e4c.c test/g02.c
bin_check_g03_SOURCES                   = src/e4c.c test/g03.c
bin_check_g04_SOURCES                   = src/e4c.c test/g04.c
bin_check_g05_SOURCES                   = src/e4c.c test/g05.c
bin_check_g06_SOURCES                   = src/e4c.c test/g06.c
bin_check_g07_SOURCES                   = src/e4c.c test/g07.c
bin_check_g08_SOURCES                   = src/e4c.c test/g08.c
bin_check_g09_SOURCES                   = src/e4c.c test/g09.c
bin_check_g10_SOURCES                   = src/e4c.c test/g10.c
bin_check_g11_SOURCES                   = src/e4c.c test/g11.c
bin_check_g12_SOURCES                   = src/e4c.c test/g12.c
bin_check_g13_SOURCES                   = src/e4c.c test/g13.c
bin_check_g14_SOURCES                   = src/e4c.c test/g14.c
bin_check_g15_SOURCES                   = src/e4c.c test/g15.c
bin_check_g16_SOURCES                   = src/e4c.c test/g16.c
bin_check_g17_SOURCES                   = src/e4c.c test/g17.c
bin_check_g18_SOURCES                   = src/e4c.c test/g18.c
bin_check_g19_SOURCES                   = src/e4c.c test/g19.c
bin_check_g20_SOURCES                   = src/e4c.c test/g20.c
bin_check_g21_SOURCES                   = src/e4c.c test/g21.c
bin_check_g22_SOURCES                   = src/e4c.c test/g22.c
bin_check_h01_SOURCES                   = src/e4c.c test/h01.c
bin_check_h02_SOURCES                   = src/e4c.c test/h02.c
bin_check_h03_SOURCES                   = src/e4c.c test/h03.c
bin_check_h04_SOURCES                   = src/e4c.c test/h04.c
bin_check_h05_SOURCES                   = src/e4c.c test/h05.c
bin_check_h06_SOURCES                   = src/e4c.c test/h06.c
bin_check_h07_SOURCES                   = src/e4c.c test/h07.c
bin_check_h08_SOURCES                   = src/e4c.c test/h08.c
bin_check_h09_SOURCES                   = src/e4c.c test/h09.c
bin_check_h10_SOURCES                   = src/e4c.c test/h10.c
bin_check_h11_SOURCES                   = src/e4c.c test/h11.c
bin_check_i01_SOURCES                   = src/e4c.c test/i01.c
bin_check_i02_SOURCES                   = src/e4c.c test/i02.c
bin_check_i03_SOURCES                   = src/e4c.c test/i03.c
bin_check_i04_SOURCES                   = src/e4c.c test/i04.c
bin_check_i05_SOURCES                   = src/e4c.c test/i05.c
bin_check_i06_SOURCES                   = src/e4c.c test/i06.c
bin_check_i07_SOURCES                   = src/e4c.c test/i07.c
bin_check_i08_SOURCES                   = src/e4c.c test/i08.c
bin_check_i09_SOURCES                   = src/e4c.c test/i09.c
bin_check_i10_SOURCES                   = src/e4c.c test/i10.c
bin_check_i11_SOURCES                   = src/e4c.c test/i11.c
bin_check_i12_SOURCES                   = src/e4c.c test/i12.c
bin_check_i13_SOURCES                   = src/e4c.c test/i13.c
bin_check_i14_SOURCES                   = src/e4c.c test/i14.c
bin_check_i15_SOURCES                   = src/e4c.c test/i15.c
bin_check_i16_SOURCES                   = src/e4c.c test/i16.c
bin_check_i17_SOURCES                   = src/e4c.c test/i17.c


# exceptions4c tests (lightweight version)

bin_check_lite_uncaught_SOURCES         = lite/src/e4c_lite.c lite/test/uncaught.c
bin_check_lite_caught_SOURCES           = lite/src/e4c_lite.c lite/test/caught.c
bin_check_lite_cleanup_SOURCES          = lite/src/e4c_lite.c lite/test/cleanup.c
bin_check_lite_hierarchy_SOURCES        = lite/src/e4c_lite.c lite/test/hierarchy.c
bin_check_lite_limits_SOURCES           = lite/src/e4c_lite.c lite/test/limits.c
bin_check_lite_overflow_SOURCES         = lite/src/e4c_lite.c lite/test/overflow.c

# coverage

coverage: check e4c.c.gcov e4c_lite.c.gcov

e4c.c.gcov:
	gcov src/e4c.c --object-directory src/*

e4c_lite.c.gcov:
	gcov lite/src/e4c_lite.c --object-directory lite/src/*
