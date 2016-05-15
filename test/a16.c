
# include "testing.h"


DEFINE_TEST(
	a16,
	"throwf(...) without beginning",
	"This test uses the library improperly, by attempting to <strong>throw an exception with a formatted message</strong>, without calling <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_THROWF",
	"ContextHasNotBegunYet"
){

	ECHO(("before_THROWF\n"));

# ifdef E4C_THROWF

		E4C_THROWF(WildException, "%s_%s", "FORMATTED", "MESSAGE");

# elif defined(HAVE_VSNPRINTF)

		e4c_exception_throw_format_(&WildException, "file", 123, "function", "%s_%s", "FORMATTED", "MESSAGE");

# else

		E4C_THROW(WildException, "No variadic macros support");

# endif

	ECHO(("after_THROWF\n"));

	return(EXIT_SUCCESS);
}
