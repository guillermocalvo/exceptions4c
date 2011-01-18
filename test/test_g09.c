
# include "testing.h"


DEFINE_TEST(
	g09,
	"Catching an abort exception",
	"This test calls <code>abort</code>; the library signal handling is enabled; the exception is caught (<code>catch(AbortException)</code>)and then the program exits.",
	"This functionality relies on the <a href=\"#requirement_z09\"><strong>platform's ability to handle signal <code>SIGABRT</code></strong></a>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"the_signal_WAS_CAUGHT"
){

	e4c_bool	caught		= e4c_false;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(e4c_true, e4c_print_exception);

	ECHO(("before_TRY_block\n"));

	E4C_TRY{

		ECHO(("before_ABORT\n"));

		abort();

		ECHO(("after_ABORT\n"));

	}E4C_CATCH(AbortException){

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
