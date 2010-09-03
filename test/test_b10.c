
# include "testing.h"


static void aux(){

	printf("before_TRY_block\n");

	try{
		printf("inside_TRY_block\n");
	}finally{
		printf("inside_FINALLY_block\n");
		goto out_of_try_block;
	}

	out_of_try_block:

	printf("after_TRY_block\n");
}

DEFINE_TEST(
	b10,
	"goto... in the middle of a finally{...} block",
	"This test uses the library in an inconsistent way, by <strong>using <code>goto</code> to jump out of a <code>finally</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	EXIT_FAILURE,
	"before_CONTEXT_END",
	"ExceptionSystemFatalError"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_CALL_FUNCTION_aux\n");

	aux();

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
