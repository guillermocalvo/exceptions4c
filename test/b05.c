
# include "testing.h"

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

# define e4c_acquire_memory malloc
# define e4c_dispose_memory(buffer, failed) free(buffer)
# define e4c_using_memory(buffer, bytes) \
    E4C_WITH(buffer, e4c_dispose_memory){ \
        buffer = e4c_acquire_memory(bytes); \
        if(buffer == NULL){ \
            E4C_THROW(NotEnoughMemoryException, \
            "Could not allocate memory for: " #buffer); \
        } \
    }E4C_USE

# pragma GCC diagnostic pop

/**
 * `e4c_using_memory` block after having ended
 *
 * This test uses the library in an inconsistent way, by attempting to start a
 * `e4c_using_memory` block, after calling `e4c_context_end`.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    char * buffer;

    TEST_EXPECTING(ContextHasNotBegunYet);

    e4c_context_begin(E4C_FALSE);

    e4c_context_end();

    e4c_using_memory(buffer, (size_t)256){

        THIS_SHOULD_NOT_HAPPEN;
    }
}
