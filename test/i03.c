
# include "testing.h"


# define DISPOSE_FOO(IGNORE1, IGNORE2)


/**
 * Reacquiring a resource
 *
 * This test tries to *acquire* a dummy resource and throws an exception. Then
 * the `catch` block tries to `reacquire` the resource up to three times. The
 * third reacquisition succeeds and then the resource is first *used* and then
 * *disposed*.
 *
 */
TEST_CASE{

    volatile int foo          = 0;
    volatile int total_acquisitions = 0;

    e4c_context_begin(E4C_FALSE);

    E4C_WITH(foo, DISPOSE_FOO){

        total_acquisitions++;

        if(total_acquisitions == 1){

            TEST_ECHO("First acquisition");

        }else{

            int reacquisitions = total_acquisitions - 1;

            TEST_DUMP("%d", reacquisitions);
        }

        E4C_THROW(RuntimeException, "Simulates an error while acquiring foo");

    }E4C_USE{

        TEST_DUMP("%d", foo);

    }E4C_CATCH(RuntimeException){

        reacquire(3);
    }

    e4c_context_end();

    TEST_DUMP("%d", total_acquisitions);

    TEST_ASSERT_EQUALS(total_acquisitions, 4);
}
