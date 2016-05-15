
# include "testing.h"
# include <signal.h>


static void * custom_initialize_handler(const e4c_exception * exception);
static E4C_DEFINE_EXCEPTION(CustomException, "This is a custom exception", RuntimeException);
static char initialization[64] = "FOOBAR";
static e4c_signal_mapping custom_mappings[2] = {
    E4C_SIGNAL_MAPPING(SIGTERM, CustomException),
    E4C_NULL_SIGNAL_MAPPING
};


/**
 * Mixing custom initialization handler and signal mappings
 *
 * This test sets both custom *initialization handler* and *signal mappings*.
 * Then a *signal* is raised and the `catch` block inspects the exception.
 *
 */
TEST_CASE{

    volatile E4C_BOOL SIGTERM_was_thrown = E4C_FALSE;

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_signal_mappings(custom_mappings);

    e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

    E4C_TRY{

        SIGTERM_was_thrown = E4C_TRUE;

        raise(SIGTERM);

        SIGTERM_was_thrown = E4C_FALSE;

    }E4C_CATCH(RuntimeException){

        const e4c_exception * caught_exception = e4c_get_exception();

        TEST_ASSERT_EQUALS(caught_exception->type, &CustomException);

        TEST_ASSERT_EQUALS(caught_exception->custom_data, initialization);
    }

    e4c_context_end();

    TEST_ASSERT(SIGTERM_was_thrown);
}

static void * custom_initialize_handler(const e4c_exception * exception){

    return(initialization);
}
