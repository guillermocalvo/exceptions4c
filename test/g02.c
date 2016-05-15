
# include "testing.h"


int zero(int dummy);
int integer = 123;


/**
 * Signal `SIGFPE`
 *
 * This test attempts to divide by zero; the library signal handling is
 * disabled, so the behavior of the program will be implementation-defined
 * (typically the program will terminate abruptly).
 *
 * This functionality relies on the platform's behavior when dividing by zero.
 *
 */
TEST_CASE{

    int divisor = 10;

    e4c_context_begin(E4C_FALSE);

    divisor = zero(integer);
    integer = integer / divisor;

    e4c_context_end();

    TEST_DUMP("%d", integer);
    TEST_DUMP("%d", divisor);
}

int zero(int dummy){

    return(dummy ? 0 : 1);
}
