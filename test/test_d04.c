
# include "testing.h"


static void aux(){

	printf("before_THROW\n");

	E4C_THROW(WildException, "Nobody will catch me.");

	printf("after_THROW\n");
}


DEFINE_TEST(
	d04,
	"Uncaught exception thrown from another function called in a try{...} block",
	"This test <strong>starts a <code>try</code> block and then calls a function, which throws an exception</strong>; there is no <code>catch</code> block to handle it.",
	NULL,
	( E4C_VERSION_THREADSAFE ? EXIT_WHATEVER : EXIT_FAILURE ),
	"before_THROW",
	"WildException"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{

		printf("before_CALL_FUNCTION_aux\n");

		aux();

		printf("after_CALL_FUNCTION_aux\n");

	}

	printf("after_TRY_block\n");

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
