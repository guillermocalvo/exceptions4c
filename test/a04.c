
# include "testing.h"


/**
 * `throwf` statement without beginning
 *
 * This test uses the library improperly, by attempting to `throw` an exception
 * with a formatted message, without calling `e4c_context_begin` first.
 *
 * The library must signal the misuse by throwing the exception
 * `ContextHasNotBegunYet`.
 *
 */
TEST_CASE{

    TEST_EXPECTING(ContextHasNotBegunYet);

# if !defined(E4C_THROWF) && !defined(HAVE_VSNPRINTF)

    TEST_SKIP("This platform does not support variadic macros or vsnprintf");

# elif defined(E4C_THROWF)

    E4C_THROWF(RuntimeException, "%s_%s", "FORMATTED", "MESSAGE");

# else

    e4c_exception_throw_format_(&RuntimeException, "file", 123, "function", "%s_%s", "FORMATTED", "MESSAGE");

# endif

}
