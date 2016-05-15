
# include "testing.h"


static int library_3rd_party(int foobar);
static void library_function(int foobar);
static int parse_exception(const e4c_exception * exception);


/**
 * A library (called by an exception-unaware client) does not catch an exception
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-unaware*, but the external function uses the
 * exception framework. So the external function uses a `e4c_reusing_context`
 * block and then a new exception context is created.
 *
 * The external function does not catch an exception, however, the external
 * function inspects the error and return an status code to its caller.
 *
 */
TEST_CASE{

    int status;

    /* The client will check the returned error code */
    status = library_3rd_party(123);

    TEST_ASSERT(status == LIBRARY_FAILURE_ILLEGAL_ARGUMENT);
}

static int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-unaware */
    TEST_ASSERT( !e4c_context_is_ready() );

    {
        e4c_reusing_context(status, E4C_ON_FAILURE(parse_exception) ){

            E4C_TRY{

                library_function(foobar);
            }
        }
    }

    /* The exception context must have finished by now */
    TEST_ASSERT( !e4c_context_is_ready() );

    return(status);
}

static void library_function(int foobar){

    /* This exception will always be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "The REUSING_CONTEXT block will mute me.");
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");
}

static int parse_exception(const e4c_exception * exception){

    if( e4c_is_instance_of(exception, &InputOutputException) ){

        return(LIBRARY_FAILURE_IO);
    }

    if( e4c_is_instance_of(exception, &IllegalArgumentException) ){

        return(LIBRARY_FAILURE_ILLEGAL_ARGUMENT);
    }

    return(LIBRARY_FAILURE);
}
