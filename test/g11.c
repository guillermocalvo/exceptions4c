
# include <signal.h>
# include "testing.h"


/**
 * Catching `TerminationException`
 *
 * This test raises `SIGTERM`; the library signal handling is enabled; the
 * exception `TerminationException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGTERM);

        TEST_FAIL("TerminationException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &TerminationException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}
