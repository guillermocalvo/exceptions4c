
# include "testing.h"


DEFINE_TEST(
	d05,
	"Uncaught exception rethrown from a catch{...} block",
	"This test <strong>starts a <code>try</code> block and then throws an exception</strong>; there is a <code>catch</code> block that handles it and then <em>rethrows</em> it.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_RETHROW",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "I'm going to be caught.");

		ECHO(("after_THROW\n"));

	}E4C_CATCH(WildException){

		ECHO(("before_RETHROW\n"));

		E4C_RETHROW("Nobody will catch me this time.");

		ECHO(("after_RETHROW\n"));
	}

	ECHO(("after_TRY_block\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
