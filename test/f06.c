
# include "testing.h"


/**
 * Only one `catch` block will handle the exception
 *
 * This test starts a `try` block, throws `IllegalArgumentException` and
 * attempts to catch it with a `catch(IllegalArgumentException)` block, but
 * there is a previous `catch(RuntimeException)` block which will eventually
 * handle it.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught1 = E4C_FALSE;
    volatile E4C_BOOL caught2 = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_THROW(IllegalArgumentException, "I'm going to be caught by the first (generic) catch block.");

    }E4C_CATCH(RuntimeException){

        caught1 = E4C_TRUE;

    }E4C_CATCH(IllegalArgumentException){

        caught2 = E4C_TRUE;
    }

    e4c_context_end();

    TEST_ASSERT(caught1);
    TEST_ASSERT(!caught2);
}
