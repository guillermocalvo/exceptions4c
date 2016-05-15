
# include "testing.h"


/**
 * `e4c_print_exception(NULL)`
 *
 * This test uses the library in an inconsistent way, by attempting to pass
 * `NULL` to `e4c_print_exception`.
 *
 * The library must signal the misuse by throwing the exception
 * `NullPointerException`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(NullPointerException);

    e4c_context_begin(E4C_FALSE);

    e4c_print_exception(NULL);

    e4c_context_end();
}
