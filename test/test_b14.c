
# include "testing.h"


DEFINE_TEST(
	b14,
	"e4c_using_context{..} after having already begun",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>e4c_using_context</code> block</strong> when the exception context is already begun. The library must signal the misuse by throwing the exception <code>ContextAlreadyBegun</code>.",
	EXIT_FAILURE,
	"before_USING_CONTEXT_block",
	"ContextAlreadyBegun"
){

	printf("before_CONTEXT_BEGIN_block\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_USING_CONTEXT_block\n");

	e4c_using_context(e4c_false, e4c_print_exception){

		printf("inside_USING_CONTEXT_block\n");
	}

	printf("before_CONTEXT_END_block\n");

	e4c_context_end();

	return(EXIT_SUCCESS);
}
