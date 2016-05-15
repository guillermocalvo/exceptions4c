
# include "testing.h"


/**
 * Signal `SIGABRT`
 *
 * This test calls `abort`; the library signal handling is disabled, so the
 * behavior of the program will be implementation-defined (typically the program
 * will terminate abruptly).
 *
 * This functionality relies on the platform's behavior when calling `abort`.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    abort();

    e4c_context_end();
}
