
# include "testing.h"


DEFINE_TEST(
	d03,
	"Uncaught exception thrown from a try{...} block",
	"This test <strong>starts a <code>try</code> block and then throws an exception</strong>; there is no <code>catch</code> block to handle it.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_THROW",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Nobody will catch me.");

		ECHO(("after_THROW\n"));

	}

	ECHO(("after_TRY_block\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
