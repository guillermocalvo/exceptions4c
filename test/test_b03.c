
# include "testing.h"

# define deallocate_buffer(resource, failed) free(resource)


DEFINE_TEST(
	b03,
	"with-use{...} after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>with... use</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_WITH_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_WITH_block\n"));

	E4C_WITH(buffer, deallocate_buffer){

		buffer = malloc( (size_t)256 );

		ECHO(("inside_WITH_block\n"));

	}E4C_USE{

		ECHO(("inside_USE_block\n"));

	}

	ECHO(("after_USE_block\n"));

	return(EXIT_SUCCESS);
}
