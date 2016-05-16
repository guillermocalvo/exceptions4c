
# include "testing.h"


/**
 * `break` statement in the middle of a `try` block
 *
 * This test uses the library in an inconsistent way, by breaking out of a `try`
 * block.
 *
 * The library must signal the misuse by throwing the exception
 * `ExceptionSystemFatalError`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ExceptionSystemFatalError);

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        TEST_ECHO("Inside `try` block...");

        /* Never jump out of a `try` block! */
        break;
    }

    e4c_context_end();
}
