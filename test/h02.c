
# include "testing.h"


int library_3rd_party(int foobar);
void library_function(int foobar);


/**
 * A library is called by a exception-aware client
 *
 * This test simulates calling an external function (as in a library function).
 * The client code is *exception-aware*, but the external function cannot simply
 * rely on that. So the external function uses a `e4c_reusing_context` block and
 * then the existing exception context is *reused*.
 *
 * No exception is thrown and the client may well ignore the error code.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    /* The client doesn't need to check the returned error code */
    (void)library_3rd_party(0);

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
            }
        }
    }

    /* The exception context must continue */
    TEST_ASSERT( e4c_context_is_ready() );

    return(status);
}

void library_function(int foobar){

    /* This exception will never be thrown */
    if(foobar){

        E4C_THROW(IllegalArgumentException, "I'm not going to be thrown.");
    }
}
