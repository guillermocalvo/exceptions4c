
# include "testing.h"


DEFINE_TEST(
	f03,
	"A parent exception cannot be caught by catching a child exception",
	"This test starts a <code>try</code> block, throws <code>ParentException</code> and attempts to catch it with a <code>catch(ChildException)</code> block. This, obviously, won't work, so the exception will be left uncaught.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_THROW",
	"ParentException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(ParentException, "You cannot catch me by catching one of my subtypes.");

	}E4C_CATCH(ChildException){

		ECHO(("inside_CATCH_block\n"));

		ECHO(("catching__%s\n", e4c_get_exception()->name));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
