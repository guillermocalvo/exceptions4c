
# include "testing.h"


DEFINE_TEST(
	g03,
	"Signal SIGABR",
	"This test calls <code>abort</code>; the library signal handling is disabled, so the behavior of the program will be implementation-defined (typically the program will terminate abruptly).",
	"This functionality relies on the <a href=\"#requirement_z06\"><strong>platform's behavior when aborting program</strong></a>.",
	EXIT_WHATEVER,
	"before_ABORT",
	ERROR_WHATEVER
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_FALSE);

	ECHO(("before_ABORT\n"));

	abort();

	/*@-unreachable@*/

	ECHO(("after_ABORT\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);

	/*@=unreachable@*/
}
