
# include "testing.h"


static void * null(int dummy);
static int integer = 123;


/**
 * Catching `BadPointerException`
 *
 * This test attempts to dereference a null pointer; the library signal handling
 * is enabled. The library will convert the signal `SIGSEGV` into the exception
 * `BadPointerException`. There is a `catch(BadPointerException)` block,
 * therefore the exception will be caught.
 *
 * This functionality relies on the platform's ability to handle signal
 * `SIGSEGV`.
 *
 */
TEST_CASE{

    volatile E4C_BOOL caught = E4C_FALSE;

    e4c_context_begin(E4C_TRUE);

    E4C_TRY{

        int * pointer = &integer;

        pointer = null(integer);
        integer = *pointer;

        TEST_FAIL("BadPointerException should have been thrown");

        TEST_DUMP("%d", integer);
        TEST_DUMP("%p", (void *)pointer);

    }E4C_CATCH(BadPointerException){

        caught = E4C_TRUE;

        TEST_ASSERT(e4c_get_exception()->type == &BadPointerException);
    }

    e4c_context_end();

    TEST_ASSERT(caught);
}

static void * null(int dummy){

    return(dummy ? NULL : &integer);
}
