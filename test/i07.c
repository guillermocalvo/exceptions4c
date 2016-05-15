
# include "testing.h"


char foobar[64] = "FOOBAR";
void * custom_initialize_handler(const e4c_exception * exception);
volatile E4C_BOOL custom_handler_was_initialized = E4C_FALSE;


/**
 * Setting a custom initialization handler
 *
 * This test sets a custom *initialization handler*. Then *throws* an exception;
 * the `catch` block inspects the exception's *custom data*.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

    E4C_TRY{

        E4C_THROW(RuntimeException, "Initialize my custom data with the result of a function");

    }E4C_CATCH(RuntimeException){

        custom_handler_was_initialized = ( strcmp(e4c_get_exception()->custom_data, foobar) == 0 );
    }

    e4c_context_end();

    TEST_ASSERT(custom_handler_was_initialized);
}

void * custom_initialize_handler(const e4c_exception * exception){

    return(&foobar);
}
