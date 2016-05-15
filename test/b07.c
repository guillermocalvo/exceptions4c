
# include "testing.h"


/**
 * `e4c_context_begin` call after having already begun
 *
 * This test uses the library in an inconsistent way, by attempting to call
 * `e4c_context_begin` twice in a row.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasAlreadyBegun`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasAlreadyBegun);

    e4c_context_begin(E4C_FALSE);

    e4c_context_begin(E4C_FALSE);
}
