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
    bin/check/test_e4c                  \
    bin/check/lite/t_uncaught           \
    bin/check/lite/t_caught             \
    bin/check/lite/t_cleanup            \
    bin/check/lite/t_hierarchy          \
    bin/check/lite/t_limits             \
    bin/check/lite/t_overflow

TESTS =                                 \
    bin/check/test_e4c                  \
    bin/check/lite/t_uncaught           \
    bin/check/lite/t_caught             \
    bin/check/lite/t_cleanup            \
    bin/check/lite/t_hierarchy          \
    bin/check/lite/t_limits             \
    bin/check/lite/t_overflow

XFAIL_TESTS =                           \
    bin/check/lite/t_uncaught           \
    bin/check/lite/t_overflow

tests: check


# exceptions4c library

lib_libe4c_a_CFLAGS                     = -Wall -Werror --pedantic -Wno-missing-braces
lib_libe4c_a_SOURCES                    = src/e4c.c


# exceptions4c library (lightweight version)

lib_libe4c_lite_a_CFLAGS                = -Wall -Werror --pedantic -Wno-missing-braces
lib_libe4c_lite_a_SOURCES               = lite/src/e4c_lite.c


# exceptions4c tests

bin_check_test_e4c_SOURCES =            \
                                        \
    src/e4c.c                           \
                                        \
    etc/e4c_rsc.c                       \
                                        \
    test/html.c                         \
    test/macros.c                       \
    test/main.c                         \
    test/run_a.c                        \
    test/run_b.c                        \
    test/run_c.c                        \
    test/run_d.c                        \
    test/run_e.c                        \
    test/run_f.c                        \
    test/run_g.c                        \
    test/run_h.c                        \
    test/run_i.c                        \
    test/run_z.c                        \
    test/run__all.c                     \
    test/suite_a.c                      \
    test/suite_b.c                      \
    test/suite_c.c                      \
    test/suite_d.c                      \
    test/suite_e.c                      \
    test/suite_f.c                      \
    test/suite_g.c                      \
    test/suite_h.c                      \
    test/suite_i.c                      \
    test/suite_z.c                      \
    test/testing.c                      \
    test/a01.c                          \
    test/a02.c                          \
    test/a03.c                          \
    test/a04.c                          \
    test/a05.c                          \
    test/a06.c                          \
    test/a07.c                          \
    test/a08.c                          \
    test/a09.c                          \
    test/a10.c                          \
    test/a11.c                          \
    test/a12.c                          \
    test/a13.c                          \
    test/a14.c                          \
    test/a15.c                          \
    test/a16.c                          \
    test/b01.c                          \
    test/b02.c                          \
    test/b03.c                          \
    test/b04.c                          \
    test/b05.c                          \
    test/b06.c                          \
    test/b07.c                          \
    test/b08.c                          \
    test/b09.c                          \
    test/b10.c                          \
    test/b11.c                          \
    test/b12.c                          \
    test/b13.c                          \
    test/b14.c                          \
    test/b15.c                          \
    test/b16.c                          \
    test/b17.c                          \
    test/b18.c                          \
    test/b19.c                          \
    test/b20.c                          \
    test/b21.c                          \
    test/b22.c                          \
    test/c01.c                          \
    test/c02.c                          \
    test/d01.c                          \
    test/d02.c                          \
    test/d03.c                          \
    test/d04.c                          \
    test/d05.c                          \
    test/e01.c                          \
    test/e02.c                          \
    test/e03.c                          \
    test/f01.c                          \
    test/f02.c                          \
    test/f03.c                          \
    test/f04.c                          \
    test/f05.c                          \
    test/f06.c                          \
    test/f07.c                          \
    test/g01.c                          \
    test/g02.c                          \
    test/g03.c                          \
    test/g04.c                          \
    test/g05.c                          \
    test/g06.c                          \
    test/g07.c                          \
    test/g08.c                          \
    test/g09.c                          \
    test/g10.c                          \
    test/g11.c                          \
    test/g12.c                          \
    test/g13.c                          \
    test/g14.c                          \
    test/g15.c                          \
    test/g16.c                          \
    test/g17.c                          \
    test/g18.c                          \
    test/g19.c                          \
    test/g20.c                          \
    test/g21.c                          \
    test/g22.c                          \
    test/g23.c                          \
    test/g24.c                          \
    test/h01.c                          \
    test/h02.c                          \
    test/h03.c                          \
    test/h04.c                          \
    test/h05.c                          \
    test/h06.c                          \
    test/h07.c                          \
    test/h08.c                          \
    test/h09.c                          \
    test/h10.c                          \
    test/h11.c                          \
    test/i01.c                          \
    test/i02.c                          \
    test/i03.c                          \
    test/i04.c                          \
    test/i05.c                          \
    test/i06.c                          \
    test/i07.c                          \
    test/i08.c                          \
    test/i09.c                          \
    test/i10.c                          \
    test/i11.c                          \
    test/i12.c                          \
    test/i13.c                          \
    test/i14.c                          \
    test/i15.c                          \
    test/z01.c                          \
    test/z02.c                          \
    test/z03.c                          \
    test/z04.c                          \
    test/z05.c                          \
    test/z06.c                          \
    test/z07.c                          \
    test/z08.c                          \
    test/z09.c                          \
    test/z10.c                          \
    test/z11.c                          \
    test/z12.c


# exceptions4c tests (lightweight version)

bin_check_lite_t_uncaught_SOURCES       = lite/src/e4c_lite.c lite/test/t_uncaught.c
bin_check_lite_t_caught_SOURCES         = lite/src/e4c_lite.c lite/test/t_caught.c
bin_check_lite_t_cleanup_SOURCES        = lite/src/e4c_lite.c lite/test/t_cleanup.c
bin_check_lite_t_hierarchy_SOURCES      = lite/src/e4c_lite.c lite/test/t_hierarchy.c
bin_check_lite_t_limits_SOURCES         = lite/src/e4c_lite.c lite/test/t_limits.c
bin_check_lite_t_overflow_SOURCES       = lite/src/e4c_lite.c lite/test/t_overflow.c
