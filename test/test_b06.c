
# include "testing.h"


DEFINE_TEST(
	b06,
	"e4c_context_end() after having already ended",
	"This test uses the library in an inconsistent way, by attempting to <strong>call <code>e4c_context_end()</code> twice in a row</strong>. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_END_again",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_CONTEXT_END_again\n"));

	e4c_context_end();

	ECHO(("after_CONTEXT_END\n"));

	return(EXIT_SUCCESS);
}
