
# include "testing.h"


/**
 * `break` statement in the middle of a `e4c_reusing_context` block
 *
 * This test uses the library in an inconsistent way, by breaking out of a
 * `e4c_reusing_context` block.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextNotEnded`.
 *
 */
TEST_CASE{

    volatile int status = 123;

    TEST_EXPECTING(ContextNotEnded);

    {
        e4c_reusing_context(status, -123){

            TEST_ECHO("Inside `e4c_reusing_context` block...");

            /* Never jump out of a `reusing` block! */
            break;
        }
    }

    TEST_DUMP("%d", status);
}
