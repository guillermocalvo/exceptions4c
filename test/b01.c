
# include "testing.h"


/**
 * `try` block after having ended
 *
 * This test uses the library in an inconsistent way, by attempting to start a
 * `try` block, after calling `e4c_context_end`.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_context_begin(E4C_FALSE);

    e4c_context_end();

    E4C_TRY{

        THIS_SHOULD_NOT_HAPPEN;
    }
}
