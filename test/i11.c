
# include "testing.h"


/**
 * Getting status (failed)
 *
 * This test retrieves the completeness of a code block. There was an uncaught
 * exceptions during the `try` block, so the status is `e4c_failed`. Finally,
 * the outter block takes care of the exception.
 *
 */
TEST_CASE{

    volatile e4c_status status = e4c_succeeded;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        E4C_TRY{

            E4C_THROW(RuntimeException, "You can't catch me!");

        }E4C_FINALLY{

            status = e4c_get_status();
        }

    }E4C_CATCH(RuntimeException){

        TEST_DUMP("%s", e4c_get_exception()->message);
    }

    e4c_context_end();

    TEST_ASSERT(status == e4c_failed);
}
