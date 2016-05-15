
# include "testing.h"


DEFINE_TEST(
	b02,
	"throw(...) after having ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>throw an exception</strong>, after having called <code>e4c_context_end()</code>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_THROW",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_THROW\n"));

	E4C_THROW(WildException, "Actually, I can't throw an exception anymore.");

	/*@-unreachable@*/

	ECHO(("after_THROW\n"));

	return(EXIT_SUCCESS);

	/*@=unreachable@*/
}
