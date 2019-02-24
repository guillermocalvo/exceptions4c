
# include "testing.h"


# define e4c_acquire_memory malloc
# define e4c_dispose_memory(buffer, failed) free(buffer)
# define e4c_using_memory(buffer, bytes) \
    E4C_WITH(buffer, e4c_dispose_memory){ \
        buffer = e4c_acquire_memory(bytes); \
        if(!buffer){ \
            E4C_THROW(NotEnoughMemoryException, NULL); \
        } \
    }E4C_USE


/**
 * `e4c_using_memory` block without beginning
 *
 * This test uses the library improperly, by attempting to start a
 * `e4c_using_memory` block, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * tmp;

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_using_memory(tmp, (size_t)256){

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        free(tmp);
# pragma GCC diagnostic pop
    }
}
