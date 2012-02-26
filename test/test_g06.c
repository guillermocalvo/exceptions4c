
# include "testing.h"


DEFINE_TEST(
	g06,
	"Abort exception",
	"This test calls <code>abort</code>; the library signal handling is enabled; there is no <code>catch</code> block. The behavior of the program will no longer be undefined: it will terminate because of the uncaught exception <code>AbortException</code>.",
	"This functionality relies on the <a href=\"#requirement_z09\"><strong>platform's ability to handle signal <code>SIGABRT</code></strong></a>.",
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_ABORT",
	"AbortException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

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
