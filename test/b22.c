
# include "testing.h"
# include <signal.h>


/**
 * Throwing `NULL`
 *
 * This test passes `NULL` to a `throw` statement.
 *
 * The library must throw `NullPointerException` instead.
 *
 */
TEST_CASE{

    volatile E4C_BOOL thrown = E4C_FALSE;
    volatile E4C_BOOL caught = E4C_FALSE;

    TEST_EXPECTING(NullPointerException);

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        thrown = E4C_TRUE;

        E4C_THROW( *( (const e4c_exception_type *)NULL ), "I see what you did there..." );

        thrown = E4C_FALSE;

    }E4C_CATCH(RuntimeException){

        caught = (e4c_get_exception()->type == &NullPointerException);
    }

    e4c_context_end();

    TEST_ASSERT(thrown);
    TEST_ASSERT(caught);
}
