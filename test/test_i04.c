
# include "testing.h"


DEFINE_TEST(
	i04,
	"Throwing an exception with a formatted message",
	"This test <em>throws</em> an exception an exception with a formatted message.",
	NULL,
	EXIT_SUCCESS,
	"before_CONTEXT_END",
	"FORMATTED_MESSAGE"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

# ifdef E4C_THROWF

		ECHO(("before_THROWF\n"));

		E4C_THROWF(WildException, "%s_%s", "FORMATTED", "MESSAGE");

# else

		ECHO(("no_VARIADIC_MACROS\n"));

		E4C_THROW(WildException, "FORMATTED_MESSAGE");

# endif

	}E4C_CATCH(WildException){

		const e4c_exception * e;

		ECHO(("inside_CATCH\n"));

		e = e4c_get_exception();

		fprintf(stderr, e->message);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
