
# include <signal.h>
# include "testing.h"


/**
 * Catching `ProgramSignal2Exception`
 *
 * This test raises `SIGUSR2`; the library signal handling is enabled; the
 * exception `ProgramSignal2Exception` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

#ifndef SIGUSR2

    TEST_SKIP("This platform does not support SIGUSR2");

#else

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGUSR2);

        TEST_FAIL("ProgramSignal2Exception should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &ProgramSignal2Exception);
    }

    e4c_context_end();

    TEST_ASSERT(caught);

#endif

}
