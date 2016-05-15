
# include "testing.h"


/**
 * Retrying a block of code
 *
 * This test *retries* a `try` block up to three times. The `retry` is performed
 * from a `finally` block, but it could also be done from a `catch` block, when
 * an exception is caught.
 *
 */
TEST_CASE{

    volatile int total_tries = 0;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        total_tries++;

        if(total_tries == 1){

            TEST_ECHO("First try");

        }else{

            int retries = total_tries - 1;

            TEST_DUMP("%d", retries);
        }

    }E4C_FINALLY{

        retry(3);
    }

    e4c_context_end();

    TEST_DUMP("%d", total_tries);

    TEST_ASSERT_EQUALS(total_tries, 4);
}
