
# include "testing.h"


volatile E4C_BOOL custom_handler_was_executed = E4C_FALSE;
void check_execution(void);
void custom_uncaught_handler(const e4c_exception * exception);


/**
 * Setting a custom uncaught handler
 *
 * This test sets a custom *uncaught handler*. Then *throws* an exception; there
 * is no `catch` block to handle it.
 *
 */
TEST_CASE{

    TEST_EXPECTING(RuntimeException);

    atexit(check_execution);

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_handlers(custom_uncaught_handler, NULL, NULL, NULL);

    E4C_THROW(RuntimeException, "You can't stop me now!");

    e4c_context_end();
}

void check_execution(void){

    TEST_X_ASSERT(custom_handler_was_executed);
}

void custom_uncaught_handler(const e4c_exception * exception){

    custom_handler_was_executed = E4C_TRUE;
}
