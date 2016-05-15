
# include "testing.h"


# define deallocate_buffer(resource, failed) free(resource)


/**
 * `with... use` block without beginning
 *
 * This test uses the library improperly, by attempting to start a `with... use`
 * block, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * tmp = NULL;

    TEST_EXPECTING(ContextHasNotBegunYet);

    E4C_WITH(tmp, deallocate_buffer){

        tmp = malloc( (size_t)256 );

    }E4C_USE{

        free(tmp);
    }
}
