
# include "testing.h"


static void * null(int dummy);
static int integer = 123;


/**
 * Bad pointer exception
 *
 * This test attempts to dereference a null pointer; the library signal handling
 * is enabled. The library will convert the signal `SIGSEGV` into the exception
 * `BadPointerException`. There is no `catch` block, therefore the program will
 * terminate because of the uncaught exception.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGSEGV`.
 *
 */
TEST_CASE{

    int * pointer = &integer;

    TEST_EXPECTING(BadPointerException);

    e4c_context_begin(E4C_TRUE);

    pointer = null(integer);
    integer = *pointer;

    e4c_context_end();

    TEST_DUMP("%d", integer);
    TEST_DUMP("%p", (void *)pointer);
}

static void * null(int dummy){

    return(dummy ? NULL : &integer);
}
