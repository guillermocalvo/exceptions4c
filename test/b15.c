
# include "testing.h"


/**
 * `catch(NULL)`
 *
 * This test uses the library in an inconsistent way, by attempting to pass
 * `NULL` to a `catch` block.
 *
 * The library must signal the misuse by throwing the exception
 * `NullPointerException`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(NullPointerException);

    e4c_context_begin(E4C_FALSE);

    try{

        E4C_THROW(RuntimeException, NULL);

    /* Never pass NULL to a catch block! */
    }catch(  *( (e4c_exception_type *)NULL )  ){

        THIS_SHOULD_NOT_HAPPEN;
    }

    e4c_context_end();
}
