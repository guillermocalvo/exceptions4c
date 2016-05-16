
# include "testing.h"


/**
 * `e4c_context_end` call after having already ended
 *
 * This test uses the library in an inconsistent way, by attempting to call
 * `e4c_context_end` twice in a row.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_context_begin(E4C_FALSE);;

    e4c_context_end();

    e4c_context_end();
}
