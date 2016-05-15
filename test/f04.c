
# include "testing.h"


void aux(volatile E4C_BOOL * flag);


/**
 * Leaking a *sibling* exception
 *
 * This test starts a `try` block, throws `IllegalArgumentException` and
 * attempts to catch it with a `catch(InputOutputException)` block. This,
 * obviously, won't work, so the exception will be left uncaught.
 *
 */
TEST_CASE{

    volatile E4C_BOOL uncaught = E4C_FALSE;

    TEST_EXPECTING(RuntimeException);

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        aux(&uncaught);

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT( e4c_get_exception()->type == &IllegalArgumentException );
    }

    TEST_ASSERT(uncaught);

    e4c_context_end();
}


void aux(volatile E4C_BOOL * flag){

    E4C_TRY{

        E4C_THROW(IllegalArgumentException, "I am not an instance of InputOutputException.");

    }E4C_CATCH(InputOutputException){

        TEST_FAIL("Block `catch(InputOutputException)` cannot handle an IllegalArgumentException");

    }E4C_FINALLY{

        *flag = ( e4c_get_status() == e4c_failed );
    }

    THIS_SHOULD_NOT_HAPPEN;

    *flag = E4C_FALSE;
}
