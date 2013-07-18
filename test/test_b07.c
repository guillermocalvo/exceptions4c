
# include "testing.h"


DEFINE_TEST(
	b07,
	"e4c_context_begin() after having already begun",
	"This test uses the library in an inconsistent way, by attempting to <strong>call <code>e4c_context_begin()</code> twice in a row</strong>. The library must signal the misuse by throwing the exception <code>ContextHasAlreadyBegun</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_BEGIN_again",
	"ContextAlreadyBegun"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_BEGIN_again\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("after_CONTEXT_BEGIN\n"));

	return(EXIT_SUCCESS);
}
