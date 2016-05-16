
# include "testing.h"


void aux(volatile E4C_BOOL * flag);


/**
 * Leaking a *parent* exception
 *
 * This test starts a `try` block, throws `RuntimeException` and attempts to
 * catch it with a `catch(IllegalArgumentException)` block. This, obviously,
 * won't work, so the exception will be left uncaught.
 *
 */
TEST_CASE{

    volatile E4C_BOOL uncaught = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        aux(&uncaught);

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT( e4c_get_exception()->type == &RuntimeException );
    }

    TEST_ASSERT(uncaught);

    e4c_context_end();
}


void aux(volatile E4C_BOOL * flag){

    E4C_TRY{

        E4C_THROW(RuntimeException, "I am not an instance of IllegalArgumentException.");

    }E4C_CATCH(IllegalArgumentException){

        TEST_FAIL("Block `catch(IllegalArgumentException)` cannot handle a RuntimeException");

    }E4C_FINALLY{

        *flag = ( e4c_get_status() == e4c_failed );
    }

    THIS_SHOULD_NOT_HAPPEN;

    *flag = E4C_FALSE;
}
