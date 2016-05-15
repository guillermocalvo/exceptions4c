
# include "testing.h"


static void another_function(volatile E4C_BOOL * flag1, volatile E4C_BOOL * flag2);
static void yet_another_function(volatile E4C_BOOL * flag2);


/**
 * Uncaught exception with a pair of `finally` blocks
 *
 * This test checks the execution of two consecutive `finally` blocks.
 *
 * The expected behavior is:
 *
 *   - The test starts a `try` block with a `catch` block.
 *   - A function is called from the `try` block.
 *     - The function starts a `try` block with a `finally` #1 block.
 *     - Another function is called from the `try` block.
 *       - The function starts a `try` block with a `finally` #2 block.
 *       - The function throws an exception from its `try` block.
 *       - There is no `catch` block to handle it.
 *       - The `finally` #2 block (of the function) is executed.
 *     - The `finally` #1 block (of the test) is executed.
 *   - The outter `catch` block catches the exception.
 *
 */
TEST_CASE{

    volatile E4C_BOOL cleanup1 = E4C_FALSE;
    volatile E4C_BOOL cleanup2 = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        another_function(&cleanup1, &cleanup2);

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT(  e4c_is_instance_of(e4c_get_exception(), &IllegalArgumentException) );
    }

    TEST_ASSERT(cleanup1);
    TEST_ASSERT(cleanup2);

    e4c_context_end();
}

static void another_function(volatile E4C_BOOL * flag1, volatile E4C_BOOL * flag2){

    E4C_TRY{

        yet_another_function(flag2);

    }E4C_FINALLY{

        *flag1 = E4C_TRUE;
    }

    *flag1 = E4C_FALSE;
}

static void yet_another_function(volatile E4C_BOOL * flag2){

    E4C_TRY{

        E4C_THROW(IllegalArgumentException, "Get me out of here.");

    }E4C_FINALLY{

        *flag2 = E4C_TRUE;
    }

    *flag2 = E4C_FALSE;
}
