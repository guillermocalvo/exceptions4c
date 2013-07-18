
# include "testing.h"

# define e4c_acquire_buffer malloc
# define e4c_dispose_buffer(resource, failed) free(resource)


DEFINE_TEST(
	a04,
	"using{...} without beginning",
	"This test uses the library improperly, by attempting to <strong>start a <code>using</code> block</strong>, without calling <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_USING_block",
	"ContextHasNotBegunYet"
){

	char * tmp;

	ECHO(("before_USING_block\n"));

	E4C_USING(buffer, tmp, ( (size_t)256 ) ){
		ECHO(("inside_USING_block\n"));
	}

	ECHO(("after_USING_block\n"));

	return(EXIT_SUCCESS);
}
