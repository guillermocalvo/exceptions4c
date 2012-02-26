
# include "testing.h"


DEFINE_TEST(
	e01,
	"Uncaught exception with a finally{...} block",
	"This test checks the execution of a <code>finally</code> block. The expected behavior is:"
		"<ol>"
		"<li>The test starts a <code>try</code> block with a <code>finally</code> block.</li>"
		"<li>The test throws an exception from inside the <code>try</code> block.</li>"
		"<li>There is no <code>catch</code> block to handle it.</li>"
		"<li>The <code>finally</code> block is executed.</li>"
		"<li>The program is terminated.</li>"
		"</ol>",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"inside_FINALLY_block",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_FINALLY_block\n"));

	E4C_TRY{

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "Nobody will catch me.");

		/*@-unreachable@*/

		ECHO(("after_THROW\n"));

		/*@=unreachable@*/

	}E4C_FINALLY{

		ECHO(("inside_FINALLY_block\n"));

	}

	ECHO(("after_TRY_FINALLY_block\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
