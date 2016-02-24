
# include <string.h>
# include <signal.h>
# include "testing.h"


DEFINE_TEST(
	g17,
	"Signal SIGSTOP",
	"This test raises <code>SIGSTOP</code>; the library signal handling is enabled; the exception <code>StopException</code> is caught and then the program exits.",
	"This functionality relies on the <strong>platform's ability to handle signals</strong>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"StopException_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	E4C_TRY{

		ECHO(("before_SIGSTOP\n"));

#ifdef SIGSTOP

		ECHO(("SIGSTOP_CANNOT_ACTUALLY_BE_HANDLED\n"));

		/* TODO: remove mapping: SIGSTOP/StopException */
		/* raise(SIGSTOP); */

#else

		ECHO(("SIGSTOP_IS_UNDEFINED_ON_THIS_PLATFORM\n"));

#endif

		throw(StopException, "This exception simulates a signal SIGSTOP");

		/*@-unreachable@*/
		ECHO(("after_SIGSTOP\n"));
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

		ECHO(("SIGSTOP_WAS_NOT_CAUGHT\n"));

	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
