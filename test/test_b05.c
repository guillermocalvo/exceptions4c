
# include "testing.h"
# include "e4c_rsc.h"


DEFINE_TEST(
	b05,
	"e4c_using_memory(...) after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>e4c_using_memory</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_USING_MEMORY_block",
	"ContextHasNotBegunYet"
){

	char * buffer;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_USING_MEMORY_block\n"));

	e4c_using_memory(buffer, (size_t)256){
		ECHO(("inside_USING_MEMORY_block\n"));
	}

	ECHO(("after_USING_MEMORY_block\n"));

	return(EXIT_SUCCESS);
}
