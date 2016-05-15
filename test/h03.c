
# include "testing.h"


static int library_3rd_party(int foobar);
static void library_function(int foobar);


/**
 * A library (called by an exception-unaware client) catches an exception
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-unaware*, but the external function uses the
 * exception framework. So the external function uses a `e4c_reusing_context`
 * block and then a new exception context is created.
 *
 * The external function catches an exception thrown by another function and
 * returns a status_code to its caller.
 *
 */
TEST_CASE{

    int status;

    /* The client will check the returned error code */
    status = library_3rd_party(123);

    TEST_ASSERT(status == LIBRARY_SUCCESS);
}

static int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-unaware */
    TEST_ASSERT( !e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                library_function(foobar);

            }E4C_CATCH(RuntimeException){

                TEST_ASSERT( e4c_is_instance_of(e4c_get_exception(), &IllegalArgumentException) );
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

        E4C_THROW(IllegalArgumentException, "The external function will catch me.");
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");
}
