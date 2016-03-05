#
# exceptions4c
#
# Copyright (c) 2016 Guillermo Calvo
# Licensed under the GNU Lesser General Public License
#

AUTOMAKE_OPTIONS = -Wall -Werror foreign subdir-objects


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

lib_libe4c_a_CFLAGS                     = --pedantic -Wall -Wextra -ansi
lib_libe4c_a_SOURCES                    = src/e4c.c


# exceptions4c library (lightweight version)

lib_libe4c_lite_a_CFLAGS                = --pedantic -Wall -Wextra -ansi
lib_libe4c_lite_a_SOURCES               = lite/src/e4c_lite.c


# exceptions4c tests

E4C_TEST_CFLAGS = --pedantic -Wall -std=c99 -coverage -O0

bin_check_test_e4c_CFLAGS               = $(E4C_TEST_CFLAGS)
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
    test/test_a01.c                     \
    test/test_a02.c                     \
    test/test_a03.c                     \
    test/test_a04.c                     \
    test/test_a05.c                     \
    test/test_a06.c                     \
    test/test_a07.c                     \
    test/test_a08.c                     \
    test/test_a09.c                     \
    test/test_a10.c                     \
    test/test_a11.c                     \
    test/test_a12.c                     \
    test/test_a13.c                     \
    test/test_a14.c                     \
    test/test_a15.c                     \
    test/test_a16.c                     \
    test/test_b01.c                     \
    test/test_b02.c                     \
    test/test_b03.c                     \
    test/test_b04.c                     \
    test/test_b05.c                     \
    test/test_b06.c                     \
    test/test_b07.c                     \
    test/test_b08.c                     \
    test/test_b09.c                     \
    test/test_b10.c                     \
    test/test_b11.c                     \
    test/test_b12.c                     \
    test/test_b13.c                     \
    test/test_b14.c                     \
    test/test_b15.c                     \
    test/test_b16.c                     \
    test/test_b17.c                     \
    test/test_b18.c                     \
    test/test_b19.c                     \
    test/test_b20.c                     \
    test/test_b21.c                     \
    test/test_b22.c                     \
    test/test_c01.c                     \
    test/test_c02.c                     \
    test/test_d01.c                     \
    test/test_d02.c                     \
    test/test_d03.c                     \
    test/test_d04.c                     \
    test/test_d05.c                     \
    test/test_e01.c                     \
    test/test_e02.c                     \
    test/test_e03.c                     \
    test/test_f01.c                     \
    test/test_f02.c                     \
    test/test_f03.c                     \
    test/test_f04.c                     \
    test/test_f05.c                     \
    test/test_f06.c                     \
    test/test_f07.c                     \
    test/test_g01.c                     \
    test/test_g02.c                     \
    test/test_g03.c                     \
    test/test_g04.c                     \
    test/test_g05.c                     \
    test/test_g06.c                     \
    test/test_g07.c                     \
    test/test_g08.c                     \
    test/test_g09.c                     \
    test/test_g10.c                     \
    test/test_g11.c                     \
    test/test_g12.c                     \
    test/test_g13.c                     \
    test/test_g14.c                     \
    test/test_g15.c                     \
    test/test_g16.c                     \
    test/test_g17.c                     \
    test/test_g18.c                     \
    test/test_g19.c                     \
    test/test_g20.c                     \
    test/test_g21.c                     \
    test/test_g22.c                     \
    test/test_g23.c                     \
    test/test_g24.c                     \
    test/test_h01.c                     \
    test/test_h02.c                     \
    test/test_h03.c                     \
    test/test_h04.c                     \
    test/test_h05.c                     \
    test/test_h06.c                     \
    test/test_h07.c                     \
    test/test_h08.c                     \
    test/test_h09.c                     \
    test/test_h10.c                     \
    test/test_h11.c                     \
    test/test_i01.c                     \
    test/test_i02.c                     \
    test/test_i03.c                     \
    test/test_i04.c                     \
    test/test_i05.c                     \
    test/test_i06.c                     \
    test/test_i07.c                     \
    test/test_i08.c                     \
    test/test_i09.c                     \
    test/test_i10.c                     \
    test/test_i11.c                     \
    test/test_i12.c                     \
    test/test_i13.c                     \
    test/test_i14.c                     \
    test/test_i15.c                     \
    test/test_z01.c                     \
    test/test_z02.c                     \
    test/test_z03.c                     \
    test/test_z04.c                     \
    test/test_z05.c                     \
    test/test_z06.c                     \
    test/test_z07.c                     \
    test/test_z08.c                     \
    test/test_z09.c                     \
    test/test_z10.c                     \
    test/test_z11.c                     \
    test/test_z12.c


# exceptions4c tests (lightweight version)

E4C_LITE_TEST_CFLAGS = --pedantic -Wall -std=c99 -coverage -O0 -I$(E4C_LITE_PATH)

bin_check_lite_t_uncaught_CFLAGS        = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_uncaught_SOURCES       = lite/test/t_uncaught.c lite/src/e4c_lite.c

bin_check_lite_t_caught_CFLAGS          = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_caught_SOURCES         = lite/test/t_caught.c lite/src/e4c_lite.c

bin_check_lite_t_cleanup_CFLAGS         = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_cleanup_SOURCES        = lite/test/t_cleanup.c lite/src/e4c_lite.c

bin_check_lite_t_hierarchy_CFLAGS       = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_hierarchy_SOURCES      = lite/test/t_hierarchy.c lite/src/e4c_lite.c

bin_check_lite_t_limits_CFLAGS          = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_limits_SOURCES         = lite/test/t_limits.c lite/src/e4c_lite.c

bin_check_lite_t_overflow_CFLAGS        = $(E4C_LITE_TEST_CFLAGS)
bin_check_lite_t_overflow_SOURCES       = lite/test/t_overflow.c lite/src/e4c_lite.c
