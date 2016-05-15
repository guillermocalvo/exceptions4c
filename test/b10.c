
# include "testing.h"


static void aux(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("before_TRY_block\n"));

	E4C_TRY{
		ECHO(("inside_TRY_block\n"));
	}E4C_FINALLY{
		ECHO(("inside_FINALLY_block\n"));
		goto out_of_try_block;
	}

	out_of_try_block:

	ECHO(("after_TRY_block\n"));
}

DEFINE_TEST(
	b10,
	"goto... in the middle of a finally{...} block",
	"This test uses the library in an inconsistent way, by <strong>using <code>goto</code> to jump out of a <code>finally</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_END",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CALL_FUNCTION_aux\n"));

	aux();

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
