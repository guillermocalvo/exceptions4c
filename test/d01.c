
# include "testing.h"


DEFINE_TEST(
	d01,
	"Uncaught exception",
	"This test <strong>throws an exception</strong>; there is no <code>catch</code> block to handle it.",
	NULL,
	IF_NOT_THREADSAFE(EXIT_FAILURE),
	"before_THROW",
	"WildException"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_THROW\n"));

	E4C_THROW(WildException, "Nobody will catch me.");

	/*@-unreachable@*/

	ECHO(("after_THROW\n"));

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	ECHO(("before_EXIT_main\n"));

	return(123);

	/*@=unreachable@*/
}
