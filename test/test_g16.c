
# include <string.h>
# include <signal.h>
# include "testing.h"


DEFINE_TEST(
	g16,
	"Signal SIGPIPE",
	"This test raises <code>SIGPIPE</code>; the library signal handling is enabled; the exception <code>BrokenPipeException</code> is caught and then the program exits.",
	"This functionality relies on the <strong>platform's ability to handle signals</strong>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"BrokenPipeException_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	E4C_TRY{

		ECHO(("before_SIGPIPE\n"));

#ifdef SIGPIPE

		raise(SIGPIPE);

#else

		ECHO(("SIGPIPE_IS_UNDEFINED_ON_THIS_PLATFORM\n"));

		throw(BrokenPipeException, "This exception simulates a signal SIGPIPE");

#endif

		/*@-unreachable@*/
		ECHO(("after_SIGPIPE\n"));
		/*@=unreachable@*/

	}E4C_CATCH(SignalException){

		ECHO(("inside_CATCH_block\n"));

		caught = E4C_TRUE;

		fprintf(stderr, "\n%s_WAS_CAUGHT\n", e4c_get_exception()->name);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(!caught){

		ECHO(("SIGPIPE_WAS_NOT_CAUGHT\n"));

	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
