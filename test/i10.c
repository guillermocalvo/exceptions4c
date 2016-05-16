
# include "testing.h"


/**
 * Getting status (recovered)
 *
 * This test retrieves the completeness of a code block. There was an exceptions
 * during the `try` block but it was *caught*, so the status is `e4c_recovered`.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_THROW(RuntimeException, "Please catch me");

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT( e4c_get_exception() );

    }E4C_FINALLY{

        TEST_ASSERT(e4c_get_status() == e4c_recovered);
    }

    e4c_context_end();
}
