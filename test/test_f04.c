
# include "testing.h"


DEFINE_TEST(
	f04,
	"A child exception cannot be caught by catching a sibling exception",
	"This test starts a <code>try</code> block, throws <code>ChildException</code> and attempts to catch it with a <code>catch(SiblingException)</code> block. This, obviously, won't work, so the exception will be left uncaught.",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_THROW",
	"ChildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(ChildException, "You cannot catch me by catching one of my supertype's subtypes.");

	}E4C_CATCH(SiblingException){

		ECHO(("inside_CATCH_block\n"));

		ECHO(("catching__%s\n", e4c_get_exception()->name));
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
