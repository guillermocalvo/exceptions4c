
# include "testing.h"


DEFINE_TEST(
	b08,
	"break... in the middle of a try{...} block",
	"This test uses the library in an inconsistent way, by <strong>breaking out of a <code>try</code> block</strong>. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	EXIT_FAILURE,
	"before_CONTEXT_END",
	"ExceptionSystemFatalError"
){

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_TRY_block\n");

	E4C_TRY{
		printf("inside_TRY_block\n");
		break;
	}

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("after_CONTEXT_END\n");

	return(EXIT_SUCCESS);
}
