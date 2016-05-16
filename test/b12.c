
# include "testing.h"


/**
 * `break` statement in the middle of a `e4c_using_context` block
 *
 * This test uses the library in an inconsistent way, by breaking out of a
 * `e4c_using_context` block.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextNotEnded`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextNotEnded);

    e4c_using_context(E4C_TRUE){

        TEST_ECHO("Inside `e4c_using_context` block...");

        /* Never jump out of a `using` block! */
        break;
    }
}
