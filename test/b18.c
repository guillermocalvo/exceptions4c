
# include "testing.h"


DEFINE_TEST(
	b18,
	"Retrying with no try{...} block",
	"This test uses the library in an inconsistent way, by attempting to <code>retry</code> without having entered a <code>try</code> block. The library must signal the misuse by throwing the exception <code>ExceptionSystemFatalError</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_RETRY",
	"ExceptionSystemFatalError"
){

	ECHO(("before_CONTEXT_BEGIN_block\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_RETRY\n"));

	E4C_RETRY(10);

	ECHO(("before_CONTEXT_END_block\n"));

	e4c_context_end();

	return(EXIT_SUCCESS);
}
