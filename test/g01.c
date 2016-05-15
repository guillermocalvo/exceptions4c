
# include "testing.h"


int * null(int dummy);
int integer = 123;


/**
 * Signal `SIGSEGV`
 *
 * This test attempts to dereference a null pointer; the library signal handling
 * is disabled, so the behavior of the program will be implementation-defined
 * (typically the program will terminate abruptly).
 *
 * This functionality relies on the platform's behavior when dereferencing a
 * null pointer.
 *
 */
TEST_CASE{

    int * pointer = &integer;

    e4c_context_begin(E4C_FALSE);

    pointer = null(integer);
    integer = *pointer;

    e4c_context_end();

    TEST_DUMP("%d", integer);
    TEST_DUMP("%p", (void *)pointer);
}

int * null(int dummy){

    return(dummy ? NULL : &integer);
}
