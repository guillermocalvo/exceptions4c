
# include "testing.h"


static int zero(int dummy);
static int integer = 123;


/**
 * Division by zero exception
 *
 * This test attempts to divide by zero; the library signal handling is enabled.
 * The library will convert the signal `SIGFPE` into the exception
 * `ArithmeticException`. There is no `catch` block, therefore the program will
 * terminate because of the uncaught exception.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGFPE`.
 *
 */
TEST_CASE{

    int divisor = 10;

    TEST_EXPECTING(ArithmeticException);

    e4c_context_begin(E4C_TRUE);

    divisor = zero(integer);
    integer = integer / divisor;

    e4c_context_end();

    TEST_DUMP("%d", integer);
    TEST_DUMP("%d", divisor);
}

static int zero(int dummy){

    return(dummy ? 0 : 1);
}
