
# include "testing.h"


static void aux(){

	printf("before_TRY_block\n");

	E4C_TRY{
		printf("inside_TRY_block\n");
		E4C_THROW(TamedException, "Hey! you cannot return in the middle of a catch block.");
	}E4C_CATCH(TamedException){
		printf("inside_CATCH_block\n");
		return;
	}

	printf("after_TRY_block\n");
}

DEFINE_TEST(
	b09,
	"return... in the middle of a catch{...} block",
	"This test uses the library in an inconsistent way, by <strong>returning from a function in the middle of a <code>catch</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
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
