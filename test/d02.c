
# include "testing.h"


static void another_function(void);


/**
 * Uncaught exception, thrown from another function
 *
 * This test calls a function which throws an exception; there is no `catch`
 * block to handle it.
 *
 */
TEST_CASE{

    TEST_EXPECTING(RuntimeException);

    e4c_context_begin(E4C_FALSE);

    another_function();

    e4c_context_end();
}


static void another_function(void){

    E4C_THROW(RuntimeException, "Nobody will catch me.");
}
