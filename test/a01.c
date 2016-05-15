
# include "testing.h"


/**
 * `try` block without beginning
 *
 * This test uses the library improperly, by attempting to start a `try` block,
 * without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    E4C_TRY{

        THIS_SHOULD_NOT_HAPPEN;
    }
}
