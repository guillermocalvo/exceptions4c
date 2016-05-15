
# include "testing.h"


/**
 * Exiting program without ending
 *
 * This test uses the library improperly, by attempting to exit the program,
 * without calling `e4c_context_end` first.
 *
 * The exception context is begun, but it never gets ended because the program
 * exits.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextNotEnded`.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);
}
