
# include "testing.h"


int library_3rd_party(int foobar);
void library_function(int foobar);


/**
 * A library is called by a exception-unaware client
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-unaware*, but the external function uses the
 * exception framework. So the external function uses a `e4c_reusing_context`
 * block and then a new exception context is created.
 *
 * No exception is thrown and the client receives an error code.
 *
 */
TEST_CASE{

    int status;

    /* The client will check the returned error code */
    status = library_3rd_party(0);

    TEST_ASSERT(status == LIBRARY_SUCCESS);
}

int library_3rd_party(int foobar){

    volatile int status = LIBRARY_SUCCESS;

    /* We know that the client is exception-unaware */
    TEST_ASSERT( !e4c_context_is_ready() );

    {
        e4c_reusing_context(status, LIBRARY_FAILURE){

            E4C_TRY{

                library_function(foobar);
            }
        }
    }

    /* The exception context must have finished by now */
    TEST_ASSERT( !e4c_context_is_ready() );

    return(status);
}

void library_function(int foobar){

    /* This exception will never be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "I'm not going to be thrown.");
    }
}
