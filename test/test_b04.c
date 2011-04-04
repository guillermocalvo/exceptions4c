
# include "testing.h"

static void * e4c_acquire_buffer(size_t bytes){

	void * buffer;

	buffer = malloc(bytes);

	return(buffer);
}

static void e4c_dispose_buffer(void * buffer, E4C_BOOL failed){

	if(failed){

		free(buffer);

		return;
	}

	free(buffer);
}


DEFINE_TEST(
	b04,
	"using{...} after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>start a <code>using</code> block</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_FAILURE,
	"before_USING_block",
	"ContextHasNotBegunYet"
){

	char * tmp;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE, e4c_print_exception);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_USING_block\n"));

	E4C_USING(buffer, tmp, (256) ){

		ECHO(("inside_USING_block\n"));

	}

	ECHO(("after_USING_block\n"));

	return(EXIT_SUCCESS);
}
