
# include "testing.h"


DEFINE_TEST_LONG_DESCRIPTION(
	e03,
	"Uncaught exception, rethrown from a catch{...} block, with a finally{...} block",
	"This test checks the execution of a <code>finally</code> block when an exception is <em>rethrown</em> from a preceding <code>catch</code> block."
	"The expected behavior is:",
		"<ol>"
		"<li>The test starts a <code>try</code> block with a <code>catch</code> block and a <code>finally</code> block.</li>"
		"<li>The test throws an exception from inside the <code>try</code> block.</li>"
		"<li>The <code>catch</code> block handles it.</li>"
		"<li>The exception is <em>rethrown</em> from inside the <code>catch</code> block.</li>"
		"<li>the <code>finally</code> block is executed.</li>"
		"<li>the program is terminated.</li>"
		"</ol>",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"inside_FINALLY_block",
	"WildException"
){

	ECHO(("beforeCONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_TRY_CATCH_FINALLY_block\n"));

	E4C_TRY{

		ECHO(("inside_TRY_block\n"));

		ECHO(("before_THROW\n"));

		E4C_THROW(WildException, "I'm going to be caught.");

		/*@-unreachable@*/

		ECHO(("after_THROW\n"));

		/*@=unreachable@*/

	}E4C_CATCH(WildException){

		ECHO(("inside_CATCH_block\n"));

		ECHO(("before_RETHROW\n"));

		E4C_RETHROW("Nobody will catch me this time.");

		/*@-unreachable@*/

		ECHO(("after_RETHROW\n"));

		/*@=unreachable@*/

	}E4C_FINALLY{

		ECHO(("inside_FINALLY_block\n"));

	}

	ECHO(("after_TRY_CATCH_FINALLY_block\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
