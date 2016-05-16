
# include "testing.h"


/**
 * Same `catch` block twice
 *
 * This test starts a `try` block, throws `RuntimeException` and attempts to
 * `catch` it twice with two `catch(RuntimeException)` blocks.
 *
 * The exception will only be caught by the first one.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught1 = E4C_FALSE;
    volatile E4C_BOOL caught2 = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_THROW(RuntimeException, "I can only be caught once for each try block.");

    }E4C_CATCH(RuntimeException){

        caught1 = E4C_TRUE;

    }E4C_CATCH(RuntimeException){

        caught2 = E4C_TRUE;
    }

    e4c_context_end();

    TEST_ASSERT(caught1);
    TEST_ASSERT(!caught2);
}
