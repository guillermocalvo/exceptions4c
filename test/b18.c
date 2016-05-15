
# include "testing.h"


/**
 * `retry` statement with no `try` block
 *
 * This test uses the library in an inconsistent way, by attempting to `retry`
 * without having entered a `try` block.
 *
 * The library must signal the misuse by throwing the exception
 * `ExceptionSystemFatalError`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ExceptionSystemFatalError);

    e4c_context_begin(E4C_FALSE);

    E4C_RETRY(10);

    e4c_context_end();
}
