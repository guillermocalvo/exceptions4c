
# include "testing.h"


static void aux(void)
/*@globals
	fileSystem,
	internalState,

	AssertionException,
	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
{

	ECHO(("before_SECOND_TRY_FINALLY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Nobody will catch me.");

		/*@-unreachable@*/

		ECHO(("after_THROW\n"));

		/*@=unreachable@*/

	}E4C_FINALLY{

		ECHO(("inside_SECOND_FINALLY_block__"));

	}

	ECHO(("after_SECOND_TRY_FINALLY_block\n"));

}

DEFINE_TEST_LONG_DESCRIPTION(
	e02,
	"Uncaught exception with a pair of finally{...} blocks",
	"This test checks the execution of two consecutive <code>finally</code> blocks. The expected behavior is:"
		"<ol>"
		"<li>The test starts a <code>try</code> block with a <code>finally</code> block.</li>"
		"<li>The test calls a function from inside the <code>try</code> block.</li>"
		"<li>The function starts a <code>try</code> block with a <code>finally</code> block.</li>"
		"<li>The function throws an exception from inside its <code>try</code> block.</li>",
		"<li>There is no <code>catch</code> block to handle it.</li>"
		"<li>The <code>finally</code> block (of the function) is executed.</li>"
		"<li>The <code>finally</code> block (of the test) is executed.</li>"
		"<li>The program is terminated.</li>"
		"</ol>",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"inside_SECOND_FINALLY_block____and_then____FIRST_FINALLY_block",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_FIRST_TRY_FINALLY_block\n"));

	E4C_TRY{

		ECHO(("before_CALL_FUNCTION_aux\n"));

		aux();

		ECHO(("after_CALL_FUNCTION_aux\n"));

	}E4C_FINALLY{

		ECHO(("__and_then____FIRST_FINALLY_block\n"));

	}

	ECHO(("after_FIRST_TRY_FINALLY_block\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));


	return(EXIT_SUCCESS);
}
