
# include "testing.h"


volatile E4C_BOOL custom_initializer_was_executed = E4C_FALSE;
void * custom_initialize_handler(const e4c_exception * exception);


/**
 * Mixing custom initialization handler and formatted message
 *
 * This test sets custom *initialization handler* and *throws* an exception with
 * a formatted message.
 *
 */
TEST_CASE{

# if !defined(E4C_THROWF) && !defined(HAVE_VSNPRINTF)

    TEST_SKIP("This platform does not support variadic macros or vsnprintf");

# else

    e4c_context_begin(E4C_FALSE);

    e4c_context_set_handlers(NULL, NULL, custom_initialize_handler, NULL);

    E4C_TRY{

#  ifdef E4C_THROWF

        E4C_THROWF(RuntimeException, "%s_%s", "FORMATTED", "MESSAGE");

#  elif defined(HAVE_VSNPRINTF)

        e4c_exception_throw_format_(&RuntimeException, "file", 123, "function", "%s_%s", "FORMATTED", "MESSAGE");

# endif

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT_STRING_EQUALS(e4c_get_exception()->message, "FORMATTED_MESSAGE");
    }

    e4c_context_end();

    TEST_ASSERT(custom_initializer_was_executed);

# endif

}

void * custom_initialize_handler(const e4c_exception * exception){

    custom_initializer_was_executed = E4C_TRUE;

    return(NULL);
}
