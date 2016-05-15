
# include <signal.h>
# include "testing.h"


/**
 * Catching `UserInterruptionException`
 *
 * This test raises `SIGINT`; the library signal handling is enabled; the
 * exception `UserInterruptionException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGINT);

        TEST_FAIL("UserInterruptionException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &UserInterruptionException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}
