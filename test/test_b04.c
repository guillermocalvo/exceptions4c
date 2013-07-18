
# include "testing.h"


static /*@only@*/ /*@null@*/ void * e4c_acquire_buffer(size_t bytes)
/*@*/
{

	void * buffer;

	buffer = calloc( (size_t)1, bytes );

	return(buffer);
}

static void e4c_dispose_buffer(/*@only@*/ void * buffer, E4C_BOOL failed)
/*@releases buffer@*/
/*@*/
{

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
	EXIT_WHATEVER,
	"before_USING_block",
	"ContextHasNotBegunYet"
){

	char * tmp;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_USING_block\n"));

	E4C_USING(buffer, tmp, ( (size_t)256 ) ){

		ECHO(("inside_USING_block\n"));

	}

	ECHO(("after_USING_block\n"));

	return(EXIT_SUCCESS);
}
