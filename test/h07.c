
# include "testing.h"


int library_3rd_party(int foobar);
void library_function(int foobar);


/**
 * A library (called by an exception-aware client) lets its client catch an exception
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-aware*, but the external function cannot simply
 * rely on that. So the external function uses a `e4c_reusing_context` block and
 * then the existing exception context is *reused*.
 *
 * The external function does not catch an exception so it is propagated to the
 * caller. The caller catches it and then the program continues.
 *
 */
TEST_CASE{

    volatile E4C_BOOL exception_caught = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

        /* The client doesn't need to check the returned error code */
        (void)library_3rd_party(123);

    }E4C_CATCH(IllegalArgumentException){

        exception_caught = E4C_TRUE;
    }

    e4c_context_end();

    TEST_ASSERT(exception_caught);
}

int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-aware */
    TEST_ASSERT( e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                library_function(foobar);
            }
        }
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");

    return(status);
}

void library_function(int foobar){

    /* This exception will always be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "The client of the external function will catch me.");
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");
}
