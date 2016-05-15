
# include "testing.h"


static char foobar[64] = "FOOBAR";
static void custom_finalize_handler(void * custom_data);
static volatile E4C_BOOL custom_handler_was_initialized = E4C_FALSE;
static volatile E4C_BOOL custom_handler_was_finalized = E4C_FALSE;


/**
 * Setting a custom finalization handler
 *
 * This test sets a custom *finalization handler*. Then *throws* an exception
 * and *catches* it.
 *
 */
TEST_CASE{

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_handlers(NULL, foobar, NULL, custom_finalize_handler);

    E4C_TRY{

        E4C_THROW(RuntimeException, "Finalize my custom data");

    }E4C_CATCH(RuntimeException){

        custom_handler_was_initialized = ( strcmp(e4c_get_exception()->custom_data, foobar) == 0 );
    }

    e4c_context_end();

    TEST_ASSERT(custom_handler_was_initialized);
    TEST_ASSERT(custom_handler_was_finalized);
}

static void custom_finalize_handler(void * custom_data){

    custom_handler_was_finalized = E4C_TRUE;
}
