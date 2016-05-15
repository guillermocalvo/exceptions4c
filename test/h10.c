
# include "testing.h"


int library_3rd_party(int foobar);
int library_4th_party(int foobar);
void library_function(int foobar);


/**
 * A library calls another library and lets its client catch a received exception
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-aware*, but the external function cannot simply
 * rely on that. So the external function uses a `e4c_reusing_context` block and
 * then the existing exception context is *reused*.
 *
 * The external function calls some function from yet another library, which
 * uses another `e4c_reusing_context` block. The exception context is *reused*
 * again.
 *
 * Then, an exception is thrown. The second external function does not catch it.
 * The exception is propagated to the first external function, which does not
 * catch it either.
 *
 * Finally, the caller of the first external function catches the exception and
 * then the program continues.
 *
 */
TEST_CASE{

    volatile E4C_BOOL exception_caught = E4C_FALSE;

    e4c_using_context(E4C_TRUE){

        E4C_TRY{

            /* The client doesn't need to check the returned error code */
            (void)library_3rd_party(123);

        }E4C_CATCH(IllegalArgumentException){

            exception_caught = E4C_TRUE;
        }
    }

    TEST_ASSERT(exception_caught);
}

int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-aware */
    TEST_ASSERT( e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                /* No need to check error code */
                (void)library_4th_party(foobar);

                TEST_FAIL("IllegalArgumentException should have been thrown");
            }
        }
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");

    return(status);
}

int library_4th_party(int foobar){

    volatile int error_code = EXIT_SUCCESS;

    /* We know that the client (ext1) is exception-aware */
    TEST_ASSERT( e4c_context_is_ready() );

    {
        e4c_reusing_context(error_code, EXIT_FAILURE){

            E4C_TRY{

                library_function(foobar);
            }

        }
    }

    return(error_code);
}

void library_function(int foobar){

    /* This exception will always be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "The client of the external function will catch me.");
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");
}
