
# include "testing.h"


/**
 * Exiting program right after having begun
 *
 * This test uses the library improperly, by attempting to exit the program
 * without calling `e4c_context_end` first.
 *
 * The tests begins two exception contexts:
 *
 * - the first one is ended properly
 * - the second one never gets ended, because the program exits.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextNotEnded`.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    e4c_context_end();

    e4c_context_begin(E4C_FALSE);
}
