
# include "testing.h"


/**
 * `catch` block without beginning
 *
 * This test uses the library improperly, by attempting to start a `catch`
 * block, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    int dummy = 1;

    TEST_EXPECTING(ContextHasNotBegunYet);

    if(--dummy){

        dummy = 123;

    }E4C_CATCH(NullPointerException){

        dummy = 321;
    }
}
