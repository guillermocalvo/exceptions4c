
# include "testing.h"

# define deallocate_buffer(resource, failed) free(resource)


DEFINE_TEST(
	a03,
	"with-use{...} without beginning",
	"This test uses the library improperly, by attempting to <strong>start a <code>with... use</code> block</strong>, without calling <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_WITH_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

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
