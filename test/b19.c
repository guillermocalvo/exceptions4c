
# include "testing.h"


/**
 * `reacquire` statement with no `with... use` block
 *
 * This test uses the library in an inconsistent way, by attempting to
 * `reacquire` without having entered a `with` block.
 *
 * The library must signal the misuse by throwing the exception
 * `ExceptionSystemFatalError`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ExceptionSystemFatalError);

    e4c_context_begin(E4C_FALSE);

    E4C_REACQUIRE(10);

    e4c_context_end();
}
