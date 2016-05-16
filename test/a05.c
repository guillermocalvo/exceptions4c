
# include "testing.h"


/**
 * `retry` statement without beginning
 *
 * This test uses the library improperly, by attempting `retry` the exception
 * context, without calling calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    E4C_RETRY(10);
}
