
# include "testing.h"


/**
 * Catching a generic exception
 *
 * This test starts a `try` block, throws `IllegalArgumentException` and catches
 * it with a `catch(RuntimeException)` block.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_using_context(E4C_TRUE){

        E4C_TRY{

            E4C_THROW(IllegalArgumentException, "I'm going to be caught.");

        }E4C_CATCH(RuntimeException){

            caught = E4C_TRUE;

        }
    }

    TEST_ASSERT(caught);
}
