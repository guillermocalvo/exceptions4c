
# include "testing.h"


# define e4c_acquire_buffer malloc
# define e4c_dispose_buffer(resource, failed) free(resource)


/**
 * `using` block without beginning
 *
 * This test uses the library improperly, by attempting to start a `using`
 * block, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * tmp = NULL;

    TEST_EXPECTING(ContextHasNotBegunYet);

    E4C_USING(buffer, tmp, ( (size_t)256 ) ){

        free(tmp);
    }
}
