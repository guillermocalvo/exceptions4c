
# include "testing.h"


/**
 * Catching a specific exception
 *
 * This test starts a `try` block, throws `IllegalArgumentException` and catches
 * it with a `catch(IllegalArgumentException)` block.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_THROW(IllegalArgumentException, "I'm going to be caught.");

    }E4C_CATCH(IllegalArgumentException){

        caught = E4C_TRUE;
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}
