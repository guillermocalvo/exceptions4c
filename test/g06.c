
# include "testing.h"


/**
 * Abort exception
 *
 * This test calls `abort`; the library signal handling is enabled. The library
 * will convert the signal `SIGABRT` into the exception `AbortException`.
 * There is no `catch` block, therefore the program will terminate because of
 * the uncaught exception.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGABRT`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(AbortException);

    e4c_context_begin(E4C_TRUE);

    abort();

    e4c_context_end();
}
