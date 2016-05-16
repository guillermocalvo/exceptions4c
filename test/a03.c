
# include "testing.h"


/**
 * `throw` statement without beginning
 *
 * This test uses the library improperly, by attempting to `throw` an exception,
 * without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    E4C_THROW(NullPointerException, "Can't throw an exception here");
}
