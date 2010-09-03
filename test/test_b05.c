
# include "testing.h"


DEFINE_TEST(
	b05,
	"e4c_using_memory(...) after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>e4c_using_memory</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	EXIT_FAILURE,
	"before_USING_MEMORY_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("before_USING_MEMORY_block\n");

	e4c_using_memory(buffer, 256){
		printf("inside_USING_MEMORY_block\n");
	}

	printf("after_USING_MEMORY_block\n");

	return(EXIT_SUCCESS);
}
