
# include "testing.h"


/**
 * Throwing an exception with a formatted message
 *
 * This test *throws* an exception an exception with a formatted message.
 */
TEST_CASE{

# if !defined(E4C_THROWF) && !defined(HAVE_VSNPRINTF)

    TEST_SKIP("This platform does not support variadic macros or vsnprintf");

# else

    e4c_context_begin(E4C_FALSE);

    E4C_TRY{

#  ifdef E4C_THROWF

        E4C_THROWF(RuntimeException, "%s_%s", "FORMATTED", "MESSAGE");

#  else

        e4c_exception_throw_format_(&RuntimeException, "file", 123, "function", "%s_%s", "FORMATTED", "MESSAGE");

#  endif

    }E4C_CATCH(RuntimeException){

        TEST_ASSERT_STRING_EQUALS(e4c_get_exception()->message, "FORMATTED_MESSAGE");
    }

    e4c_context_end();

# endif

}
