
# include "testing.h"


DEFINE_TEST(
	e03,
	"Uncaught exception, rethrown from a catch{...} block, with a finally{...} block",
	"This test checks the execution of a <code>finally</code> block when an exception is <em>rethrown</em> from a preceding <code>catch</code> block. The expected behavior is:"
		"<ol>"
		"<li>The test starts a <code>try</code> block with a <code>catch</code> block and a <code>finally</code> block.</li>"
		"<li>The test throws an exception from inside the <code>try</code> block.</li>"
		"<li>The <code>catch</code> block handles it.</li>"
		"<li>The exception is <em>rethrown</em> from inside the <code>catch</code> block.</li>"
		"<li>the <code>finally</code> block is executed.</li>"
		"<li>the program is terminated.</li>"
		"</ol>",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"inside_FINALLY_block",
	"WildException"
){

	printf("beforeCONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_CATCH_FINALLY_block\n");

	E4C_TRY{

		printf("inside_TRY_block\n");

		printf("before_THROW\n");

		E4C_THROW(WildException, "I'm going to be caught.");

		printf("after_THROW\n");

	}E4C_CATCH(WildException){

		printf("inside_CATCH_block\n");

		printf("before_RETHROW\n");

		E4C_THROW(*e4c_get_exception(), "Nobody will catch me this time.");

		printf("after_RETHROW\n");

	}E4C_FINALLY{

		printf("inside_FINALLY_block\n");

	}

	printf("after_TRY_CATCH_FINALLY_block\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
