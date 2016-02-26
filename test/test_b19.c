
# include "testing.h"


DEFINE_TEST(
	b19,
	"Reacquiring with no with{...} block",
	"This test uses the library in an inconsistent way, by attempting to <code>reacquire</code> without having entered a <code>WITH</code> block. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_REACQUIRE",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN_block\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_REACQUIRE\n"));

	E4C_REACQUIRE(10);

	ECHO(("before_CONTEXT_END_block\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
