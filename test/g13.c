
# include <signal.h>
# include "testing.h"


/**
 * Catching `SignalAlarmException`
 *
 * This test raises `SIGALRM`; the library signal handling is enabled; the
 * exception `SignalAlarmException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

#ifndef SIGALRM

    TEST_SKIP("This platform does not support SIGALRM");

#else

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGALRM);

        TEST_FAIL("SignalAlarmException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &SignalAlarmException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);

#endif

}
