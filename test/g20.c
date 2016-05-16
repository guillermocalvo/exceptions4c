
# include <signal.h>
# include "testing.h"


/**
 * Catching `UserBreakException`
 *
 * This test raises `SIGBREAK`; the library signal handling is enabled; the
 * exception `UserBreakException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

#ifndef SIGBREAK

    TEST_SKIP("This platform does not support SIGBREAK");

#else

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGBREAK);

        TEST_FAIL("UserBreakException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &UserBreakException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);

#endif

}
