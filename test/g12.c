
# include <string.h>
# include <signal.h>
# include "testing.h"


DEFINE_TEST(
	g12,
	"Signal SIGINT",
	"This test raises <code>SIGINT</code>; the library signal handling is enabled; the exception <code>UserInterruptionException</code> is caught and then the program exits.",
	"This functionality relies on the <strong>platform's ability to handle signals</strong>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"UserInterruptionException_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	E4C_TRY{

		ECHO(("before_SIGINT\n"));

		raise(SIGINT);

		/*@-unreachable@*/

		ECHO(("after_SIGINT\n"));

		/*@=unreachable@*/

	}E4C_CATCH(ControlSignalException){

		ECHO(("inside_CATCH_block\n"));

		caught = E4C_TRUE;

		fprintf(stderr, "\n%s_WAS_CAUGHT\n", e4c_get_exception()->name);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(!caught){

		fprintf(stderr, "\nSIGINT_WAS_NOT_CAUGHT\n");
	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
