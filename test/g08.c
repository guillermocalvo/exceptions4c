
# include "testing.h"


static int zero(int dummy);
static int integer = 123;


/**
 * Catching `ArithmeticException`
 *
 * This test attempts to divide by zero; the library signal handling is enabled.
 * The library will convert the signal `SIGFPE` into the exception
 * `ArithmeticException`. There is a `catch(SignalException)` block, therefore
 * the exception will be caught.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGFPE`.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        int divisor = 10;

        divisor = zero(integer);
        integer = integer / divisor;

        TEST_DUMP("%d", integer);

        TEST_FAIL("ArithmeticException should have been thrown");

    }E4C_CATCH(SignalException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &ArithmeticException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}

static int zero(int dummy){

    return(dummy ? 0 : 1);
}
