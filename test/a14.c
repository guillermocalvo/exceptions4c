
# include "testing.h"


/**
 * `e4c_context_get_signal_mappings` call without beginning
 *
 * This test uses the library improperly, by attempting to call
 * `e4c_context_get_signal_mappings`, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

    (void)e4c_context_get_signal_mappings();
}
