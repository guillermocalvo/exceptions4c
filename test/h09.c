
# include "testing.h"


static int library_3rd_party(int foobar);
static int library_4th_party(int foobar);
static void library_function(int foobar);


/**
 * A library (called by an exception-aware client) calls another library
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
 * When the original caller continues, the exception context keeps being ready.
 *
 */
TEST_CASE{

    int status = LIBRARY_FAILURE;

    e4c_using_context(E4C_TRUE){

        /* The client will check the returned error code */
        status = library_3rd_party(0);
    }

    TEST_ASSERT(status == LIBRARY_SUCCESS);
}

static int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-aware */
    TEST_ASSERT( e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                /* No need to check error code */
                (void)library_4th_party(foobar);
            }
        }
    }

    /* The exception context must continue */
    TEST_ASSERT( e4c_context_is_ready() );

    return(status);
}

static int library_4th_party(int foobar){

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

    /* The exception context must continue */
    TEST_ASSERT( e4c_context_is_ready() );

    return(error_code);
}

static void library_function(int foobar){

    /* This exception will never be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "I'm not going to be thrown.");
    }
}
