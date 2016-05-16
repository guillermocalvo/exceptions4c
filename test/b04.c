
# include "testing.h"


void * e4c_acquire_buffer(size_t bytes);
void e4c_dispose_buffer(void * buffer, E4C_BOOL failed);


/**
 * `using` block after having ended
 *
 * This test uses the library in an inconsistent way, by attempting to start a
 * `using` block, after calling `e4c_context_end`.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * tmp = NULL;

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_context_begin(E4C_FALSE);

    e4c_context_end();

    E4C_USING(buffer, tmp, ( (size_t)256 ) ){

        THIS_SHOULD_NOT_HAPPEN;
    }
}


void * e4c_acquire_buffer(size_t bytes){

    return( calloc( (size_t)1, bytes ) );
}

void e4c_dispose_buffer(void * buffer, E4C_BOOL failed){

    (void)failed;

    free(buffer);
}
