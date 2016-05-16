
# include <signal.h>
# include "testing.h"


/**
 * Catching `SignalChildException`
 *
 * This test raises `SIGCHLD`; the library signal handling is enabled; the
 * exception `SignalChildException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

#ifndef SIGCHLD

    TEST_SKIP("This platform does not support SIGCHLD");

#else

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGCHLD);

        TEST_FAIL("SignalChildException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &SignalChildException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);

#endif

}
