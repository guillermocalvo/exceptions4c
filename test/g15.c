
# include <string.h>
# include <signal.h>
# include "testing.h"


DEFINE_TEST(
	g15,
	"Signal SIGTRAP",
	"This test raises <code>SIGTRAP</code>; the library signal handling is enabled; the exception <code>SignalTrapException</code> is caught and then the program exits.",
	"This functionality relies on the <strong>platform's ability to handle signals</strong>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"SignalTrapException_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	E4C_TRY{

		ECHO(("before_SIGTRAP\n"));

#ifdef SIGTRAP

		raise(SIGTRAP);

#else

		ECHO(("SIGTRAP_IS_UNDEFINED_ON_THIS_PLATFORM\n"));

		throw(SignalTrapException, "This exception simulates a signal SIGTRAP");

#endif

		/*@-unreachable@*/
		ECHO(("after_SIGTRAP\n"));
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

		ECHO(("SIGTRAP_WAS_NOT_CAUGHT\n"));

	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
