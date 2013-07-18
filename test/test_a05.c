
# include "testing.h"
# include "e4c_rsc.h"


DEFINE_TEST(
	a05,
	"e4c_using_memory{...} without beginning",
	"This test uses the library improperly, by attempting to <strong>start a <code>e4c_using_memory</code> block</strong>, without calling <code>e4c_context_begin</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_USING_MEMORY_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	ECHO(("before_USING_MEMORY_block\n"));

	e4c_using_memory(buffer, (size_t)256){
		ECHO(("inside_USING_MEMORY_block\n"));
	}

	ECHO(("after_USING_MEMORY_block\n"));

	return(EXIT_SUCCESS);
}
