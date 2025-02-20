#
# exceptions4c
#
# Copyright (c) 2025 Guillermo Calvo
# Licensed under the Apache License, Version 2.0
#

AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = -Wall -Werror --pedantic -Wno-missing-braces -Wno-dangling-else -coverage -O0 -I$(EXCEPTIONS4C_PATH)


# Library

EXCEPTIONS4C_PATH = src
EXCEPTIONS4C_LIBRARY = lib/libexceptions4c.a


# Install

lib_LIBRARIES = $(EXCEPTIONS4C_LIBRARY)

include_HEADERS = src/exceptions4c.h


# Documentation

docsdir = $(datadir)/docs/exceptions4c
docs_DATA = docs/*


# Cleanup

CLEANFILES =                                \
    *.log                                   \
    *.gcda                                  \
    *.gcno                                  \
    *.gcov                                  \
    *.stackdump                             \
    src/*.gcda                              \
    src/*.gcno                              \
    src/*.gcov                              \
    tests/*.gcda                            \
    tests/*.gcno                            \
    tests/*.gcov


# Check

check_PROGRAMS =                            \
    bin/check/catch-all                     \
    bin/check/catch-duplicate               \
    bin/check/catch-generic                 \
    bin/check/catch-sigint                  \
    bin/check/catch-sigsegv                 \
    bin/check/catch-sigterm                 \
    bin/check/catch-specific                \
    bin/check/catch-unordered               \
    bin/check/examples/customization        \
    bin/check/examples/pet-store            \
    bin/check/examples/pthreads             \
    bin/check/examples/signals              \
    bin/check/examples/uncaught-handler     \
    bin/check/finally                       \
    bin/check/get-exception                 \
    bin/check/handler-finalize              \
    bin/check/handler-initialize            \
    bin/check/handler-terminate             \
    bin/check/handler-uncaught              \
    bin/check/is-uncaught                   \
    bin/check/panic-block-catch             \
    bin/check/panic-block-next              \
    bin/check/panic-block-try               \
    bin/check/panic-context                 \
    bin/check/panic-dangling                \
    bin/check/panic-reacquire               \
    bin/check/panic-retry                   \
    bin/check/panic-try                     \
    bin/check/reacquire                     \
    bin/check/retry                         \
    bin/check/throw-cause                   \
    bin/check/throw-format                  \
    bin/check/throw-suppressed              \
    bin/check/throw-uncaught-1              \
    bin/check/throw-uncaught-2              \
    bin/check/with-use

TESTS =                                     \
    bin/check/catch-all                     \
    bin/check/catch-duplicate               \
    bin/check/catch-generic                 \
    bin/check/catch-sigint                  \
    bin/check/catch-sigsegv                 \
    bin/check/catch-sigterm                 \
    bin/check/catch-specific                \
    bin/check/catch-unordered               \
    bin/check/examples/customization        \
    bin/check/examples/pet-store            \
    bin/check/examples/pthreads             \
    bin/check/examples/signals              \
    bin/check/examples/uncaught-handler     \
    bin/check/finally                       \
    bin/check/get-exception                 \
    bin/check/handler-finalize              \
    bin/check/handler-initialize            \
    bin/check/handler-terminate             \
    bin/check/handler-uncaught              \
    bin/check/is-uncaught                   \
    bin/check/panic-block-catch             \
    bin/check/panic-block-next              \
    bin/check/panic-block-try               \
    bin/check/panic-context                 \
    bin/check/panic-dangling                \
    bin/check/panic-reacquire               \
    bin/check/panic-retry                   \
    bin/check/panic-try                     \
    bin/check/reacquire                     \
    bin/check/retry                         \
    bin/check/throw-cause                   \
    bin/check/throw-format                  \
    bin/check/throw-suppressed              \
    bin/check/throw-uncaught-1              \
    bin/check/throw-uncaught-2              \
    bin/check/with-use

XFAIL_TESTS =                               \
    bin/check/examples/uncaught-handler     \
    bin/check/panic-block-catch             \
    bin/check/panic-block-next              \
    bin/check/panic-block-try               \
    bin/check/panic-context                 \
    bin/check/panic-dangling                \
    bin/check/panic-reacquire               \
    bin/check/panic-retry                   \
    bin/check/panic-try                     \
    bin/check/throw-uncaught-1              \
    bin/check/throw-uncaught-2

tests: check


# Library

lib_libexceptions4c_a_CFLAGS                = -Wall -Werror --pedantic -Wno-missing-braces -I$(EXCEPTIONS4C_PATH)
lib_libexceptions4c_a_SOURCES               = src/exceptions4c.c


# Tests

bin_check_catch_all_SOURCES                 = src/exceptions4c.c tests/catch-all.c
bin_check_catch_duplicate_SOURCES           = src/exceptions4c.c tests/catch-duplicate.c
bin_check_catch_generic_SOURCES             = src/exceptions4c.c tests/catch-generic.c
bin_check_catch_sigint_SOURCES              = src/exceptions4c.c tests/catch-sigint.c
bin_check_catch_sigsegv_SOURCES             = src/exceptions4c.c tests/catch-sigsegv.c
bin_check_catch_sigterm_SOURCES             = src/exceptions4c.c tests/catch-sigterm.c
bin_check_catch_specific_SOURCES            = src/exceptions4c.c tests/catch-specific.c
bin_check_catch_unordered_SOURCES           = src/exceptions4c.c tests/catch-unordered.c
bin_check_finally_SOURCES                   = src/exceptions4c.c tests/finally.c
bin_check_get_exception_SOURCES             = src/exceptions4c.c tests/get-exception.c
bin_check_handler_finalize_SOURCES          = src/exceptions4c.c tests/handler-finalize.c
bin_check_handler_initialize_SOURCES        = src/exceptions4c.c tests/handler-initialize.c
bin_check_handler_terminate_SOURCES         = src/exceptions4c.c tests/handler-terminate.c
bin_check_handler_uncaught_SOURCES          = src/exceptions4c.c tests/handler-uncaught.c
bin_check_is_uncaught_SOURCES               = src/exceptions4c.c tests/is-uncaught.c
bin_check_panic_block_catch_SOURCES         = src/exceptions4c.c tests/panic-block-catch.c
bin_check_panic_block_next_SOURCES          = src/exceptions4c.c tests/panic-block-next.c
bin_check_panic_block_try_SOURCES           = src/exceptions4c.c tests/panic-block-try.c
bin_check_panic_context_SOURCES             = src/exceptions4c.c tests/panic-context.c
bin_check_panic_dangling_SOURCES            = src/exceptions4c.c tests/panic-dangling.c
bin_check_panic_reacquire_SOURCES           = src/exceptions4c.c tests/panic-reacquire.c
bin_check_panic_retry_SOURCES               = src/exceptions4c.c tests/panic-retry.c
bin_check_panic_try_SOURCES                 = src/exceptions4c.c tests/panic-try.c
bin_check_reacquire_SOURCES                 = src/exceptions4c.c tests/reacquire.c
bin_check_retry_SOURCES                     = src/exceptions4c.c tests/retry.c
bin_check_throw_cause_SOURCES               = src/exceptions4c.c tests/throw-cause.c
bin_check_throw_format_SOURCES              = src/exceptions4c.c tests/throw-format.c
bin_check_throw_suppressed_SOURCES          = src/exceptions4c.c tests/throw-suppressed.c
bin_check_throw_uncaught_1_SOURCES          = src/exceptions4c.c tests/throw-uncaught-1.c
bin_check_throw_uncaught_2_SOURCES          = src/exceptions4c.c tests/throw-uncaught-2.c
bin_check_with_use_SOURCES                  = src/exceptions4c.c tests/with-use.c

# Examples

bin_check_examples_customization_LDADD      = $(EXCEPTIONS4C_LIBRARY)
bin_check_examples_customization_SOURCES    = examples/customization.c
bin_check_examples_pet_store_LDADD          = $(EXCEPTIONS4C_LIBRARY)
bin_check_examples_pet_store_SOURCES        = examples/pet-store.c
bin_check_examples_pthreads_LDADD           = $(EXCEPTIONS4C_LIBRARY)
bin_check_examples_pthreads_SOURCES         = examples/pthreads.c
bin_check_examples_signals_LDADD            = $(EXCEPTIONS4C_LIBRARY)
bin_check_examples_signals_SOURCES          = examples/signals.c
bin_check_examples_uncaught_handler_LDADD   = $(EXCEPTIONS4C_LIBRARY)
bin_check_examples_uncaught_handler_SOURCES = examples/uncaught-handler.c


# Coverage

coverage: exceptions4c.c.gcov

coverage-report: docs/html/coverage/index.html

docs/html/coverage/index.html: coverage.info
	genhtml coverage.info --output-directory docs/html/coverage

coverage.info: exceptions4c.c.gcov
	lcov --ignore-errors mismatch --capture --directory . --output-file coverage.info

exceptions4c.c.gcov: src/exceptions4c.gcda
	gcov --verbose src/exceptions4c.c

src/exceptions4c.gcda: check


# Generate documentation

docs: docs/html/index.html

docs/html/index.html: docs/Doxyfile docs/doxygen-awesome.css docs/doxygen-awesome-sidebar-only.css
	doxygen docs/Doxyfile

docs/doxygen-awesome.css:
	wget --verbose --output-document docs/doxygen-awesome.css https://github.com/jothepro/doxygen-awesome-css/raw/refs/heads/main/doxygen-awesome.css

docs/doxygen-awesome-sidebar-only.css:
	wget --verbose --output-document docs/doxygen-awesome-sidebar-only.css https://github.com/jothepro/doxygen-awesome-css/raw/refs/heads/main/doxygen-awesome-sidebar-only.css
