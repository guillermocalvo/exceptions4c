
# include "testing.h"


int library_3rd_party(int foobar);
void library_function(int foobar);


/**
 * A library (called by an exception-aware client) catches an exception
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-aware*, but the external function cannot simply
 * rely on that. So the external function uses a `e4c_reusing_context` block and
 * then the existing exception context is *reused*.
 *
 * The external function catches an exception thrown by another function and
 * returns control to its caller.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    /* The client doesn't need to check the returned error code */
    (void)library_3rd_party(123);

    e4c_context_end();
}

int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-aware */
    TEST_ASSERT( e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                library_function(foobar);

            }E4C_CATCH(IllegalArgumentException){

                TEST_ASSERT( e4c_is_instance_of(e4c_get_exception(), &IllegalArgumentException) );
            }
        }
    }

    /* The exception context must continue */
    TEST_ASSERT( e4c_context_is_ready() );

    return(status);
}

void library_function(int foobar){

    /* This exception will always be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "The external function will catch me.");
    }

    TEST_FAIL("IllegalArgumentException should have been thrown");
}
