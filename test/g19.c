
# include <signal.h>
# include "testing.h"


/**
 * Catching `UserQuitException`
 *
 * This test raises `SIGQUIT`; the library signal handling is enabled; the
 * exception `UserQuitException` is caught and then the program exits.
 *
 * This functionality relies on the platform's ability to handle signals.
 *
 */
TEST_CASE{

#ifndef SIGQUIT

    TEST_SKIP("This platform does not support SIGQUIT");

#else

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        raise(SIGQUIT);

        TEST_FAIL("UserQuitException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &UserQuitException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);

#endif

}
