
# include "testing.h"


DEFINE_TEST(
	g07,
	"Catching a null pointer exception",
	"This test attempts to dereference a null pointer; the library signal handling is enabled; the exception is caught (<code>catch(BadPointerException)</code>) and then the program exits.",
	"This functionality relies on the <a href=\"#requirement_z07\"><strong>platform's ability to handle signal <code>SIGSEGV</code></strong></a>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"the_signal_WAS_CAUGHT"
){

	e4c_bool	caught		= e4c_false;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{
		int *		null_pointer = NULL;
		int			integer;

		ECHO(("before_NULL_POINTER\n"));

		/* some smartypants compilers might need to be fooled */
		/* if(null_pointer != &integer) null_pointer = NULL; */

		integer = *null_pointer;

		ECHO(("after_NULL_POINTER_%d\n", integer));

		if(integer) integer = 0;

	}E4C_CATCH(BadPointerException){

		ECHO(("inside_CATCH_block\n"));

		caught = e4c_true;

		ECHO(("catching__%s\n", e4c_get_exception()->name));

	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(caught){
		fprintf(stderr, "\nthe_signal_WAS_CAUGHT\n");
	}else{
		fprintf(stderr, "\nthe_signal_WAS_NOT_CAUGHT\n");
	}

	fflush(stderr);

	return(EXIT_SUCCESS);
}
