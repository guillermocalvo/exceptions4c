
# include "testing.h"


void another_function(volatile E4C_BOOL * flag);


/**
 * Uncaught exception with a `finally` block
 *
 * This test checks the execution of a `finally` block.
 *
 * The expected behavior is:
 *
 *   - The test starts a `try` block with a `catch` block.
 *   - A function is called from the `try` block.
 *     - The function starts a `try` block with a `finally` block.
 *     - An exception is thrown from the `try` block.
 *     - There is no `catch` block to handle it.
 *     - The `finally` block is executed.
 *   - The outter `catch` block catches the exception.
 *
 */
TEST_CASE{

    volatile E4C_BOOL cleanup = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        another_function(&cleanup);

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT(  e4c_is_instance_of(e4c_get_exception(), &IllegalArgumentException) );
    }

    TEST_ASSERT(cleanup);

    e4c_context_end();
}

void another_function(volatile E4C_BOOL * flag){

    E4C_TRY{

        E4C_THROW(IllegalArgumentException, "Get me out of here.");

    }E4C_FINALLY{

        *flag = E4C_TRUE;
    }

    *flag = E4C_FALSE;
}
