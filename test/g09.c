
# include "testing.h"


/**
 * Catching `AbortException`
 *
 * This test calls `abort`; the library signal handling is enabled. The library
 * will convert the signal `SIGABRT` into the exception `AbortException`.
 * There is a `catch(AbortException)` block, therefore the exception will be
 * caught.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGABRT`.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        abort();

        TEST_FAIL("AbortException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &AbortException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}
