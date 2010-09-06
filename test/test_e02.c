
# include "testing.h"


static void aux(){

	printf("before_SECOND_TRY_FINALLY_block\n");

	E4C_TRY{

		printf("before_THROW\n");

		E4C_THROW(WildException, "Nobody will catch me.");

		printf("after_THROW\n");

	}E4C_FINALLY{

		printf("inside_SECOND_FINALLY_block__");

	}

	printf("after_SECOND_TRY_FINALLY_block\n");

}

DEFINE_TEST(
	e02,
	"Uncaught exception with a pair of finally{...} blocks",
	"This test checks the execution of two consecutive <code>finally</code> blocks. The expected behavior is:"
		"<ol>"
		"<li>The test starts a <code>try</code> block with a <code>finally</code> block.</li>"
		"<li>The test calls a function from inside the <code>try</code> block.</li>"
		"<li>The function starts a <code>try</code> block with a <code>finally</code> block.</li>"
		"<li>The function throws an exception from inside its <code>try</code> block.</li>"
		"<li>There is no <code>catch</code> block to handle it.</li>"
		"<li>The <code>finally</code> block (of the function) is executed.</li>"
		"<li>The <code>finally</code> block (of the test) is executed.</li>"
		"<li>The program is terminated.</li>"
		"</ol>",
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"inside_SECOND_FINALLY_block____and_then____FIRST_FINALLY_block",
	"WildException"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_FIRST_TRY_FINALLY_block\n");

	E4C_TRY{

		printf("before_CALL_FUNCTION_aux\n");

		aux();

		printf("after_CALL_FUNCTION_aux\n");

	}E4C_FINALLY{

		printf("__and_then____FIRST_FINALLY_block\n");

	}

	printf("after_FIRST_TRY_FINALLY_block\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");


	return(EXIT_SUCCESS);
}
