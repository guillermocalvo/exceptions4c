
# include "testing.h"

# define deallocate_buffer(resource, failed) free(resource)


DEFINE_TEST(
	b03,
	"with-use{...} after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>with... use</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_FAILURE,
	"before_WITH_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	printf("before_CONTEXT_BEGIN\n");

	e4c_context_begin(e4c_true, e4c_print_exception);

	printf("before_CONTEXT_END\n");

	e4c_context_end();

	printf("before_WITH_block\n");

	E4C_WITH(buffer, deallocate_buffer){

		buffer = malloc(256);

		printf("inside_WITH_block\n");

	}E4C_USE{

		printf("inside_USE_block\n");

	}

	printf("after_USE_block\n");

	return(EXIT_SUCCESS);
}
