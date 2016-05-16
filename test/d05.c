
# include "testing.h"


/**
 * Uncaught exception, rethrown from `catch` block
 *
 * This test starts a `try` block and then throws an exception; there is a
 * `catch` block that handles it and then *rethrows* it.
 *
 */
TEST_CASE{

    TEST_EXPECTING(RuntimeException);

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_THROW(RuntimeException, "I'm going to be caught.");

    }E4C_CATCH(RuntimeException){

        E4C_RETHROW("Nobody will catch me this time.");
    }

    e4c_context_end();
}
