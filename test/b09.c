
# include "testing.h"


void aux(void);


/**
 * `return` statement in the middle of a `catch` block
 *
 * This test uses the library in an inconsistent way, by returning from a
 * function in the middle of a `catch` block.
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

        E4C_THROW(RuntimeException, NULL);

    }E4C_CATCH(RuntimeException){

        TEST_ECHO("Inside `catch` block...");

        /* Never jump out of a `catch` block! */
        return;
    }
}
