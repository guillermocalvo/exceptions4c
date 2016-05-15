
# include "testing.h"


void aux(void);

/**
 * `goto` statement in the middle of a `finally` block
 *
 * This test uses the library in an inconsistent way, by using `goto` to jump
 * out of a `finally` block.
 *
 * The library must signal the misuse by throwing the exception
 * `ExceptionSystemFatalError`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ExceptionSystemFatalError);

    e4c_context_begin(E4C_FALSE);

    aux();

    e4c_context_end();
}


void aux(void){

    E4C_TRY{

        TEST_ECHO("Inside `try` block...");

    }E4C_FINALLY{

        TEST_ECHO("Inside `finally` block...");

        /* Never jump out of a `finally` block! */
        goto out_of_finally_block;
    }

    out_of_finally_block:

    TEST_ECHO("Out of `finally` block...");
}
