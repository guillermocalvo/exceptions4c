
# include "testing.h"


# define deallocate_buffer(resource, failed) free(resource)


/**
 * `with... use` block after having ended
 *
 * This test uses the library in an inconsistent way, by attempting to start a
 * `with... use` block, after calling `e4c_context_end`.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * buffer = NULL;

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_context_begin(E4C_FALSE);

    e4c_context_end();

    E4C_WITH(buffer, deallocate_buffer){

        buffer = malloc( (size_t)256 );

    }E4C_USE{

        THIS_SHOULD_NOT_HAPPEN;
    }
}
