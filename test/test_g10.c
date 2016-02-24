
# include <string.h>
# include <signal.h>
# include "testing.h"


DEFINE_TEST(
	g10,
	"Signal SIGILL",
	"This test raises <code>SIGILL</code>; the library signal handling is enabled; the exception <code>IllegalInstructionException</code> is caught and then the program exits.",
	"This functionality relies on the <strong>platform's ability to handle signals</strong>.",
	EXIT_SUCCESS,
	"after_CONTEXT_END",
	"IllegalInstructionException_WAS_CAUGHT"
){

	E4C_BOOL	caught		= E4C_FALSE;

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	E4C_TRY{

		ECHO(("before_SIGILL\n"));

		raise(SIGILL);

		/*@-unreachable@*/

		ECHO(("after_SIGILL\n"));

		/*@=unreachable@*/

	}E4C_CATCH(IllegalInstructionException){

		ECHO(("inside_CATCH_block\n"));

		caught = E4C_TRUE;

		fprintf(stderr, "\n%s_WAS_CAUGHT\n", e4c_get_exception()->name);
	}

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	if(!caught){

		fprintf(stderr, "\nSIGILL_WAS_NOT_CAUGHT\n");
	}

	(void)fflush(stderr);

	return(EXIT_SUCCESS);
}
