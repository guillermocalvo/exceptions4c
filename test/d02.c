
# include "testing.h"


static void aux(void)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("before_THROW\n"));

	E4C_THROW(WildException, "Nobody will catch me.");

	/*@-unreachable@*/

	ECHO(("after_THROW\n"));

	/*@=unreachable@*/
}


DEFINE_TEST(
	d02,
	"Uncaught exception thrown from another function",
	"This test <strong>calls a function which throws an exception</strong>; there is no <code>catch</code> block to handle it.",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_THROW",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CALL_FUNCTION_aux\n"));

	aux();

	ECHO(("after_CALL_FUNCTION_aux\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
