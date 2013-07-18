
# include "testing.h"


DEFINE_TEST(
	c02,
	"Exiting program right after having begun",
	"This test uses the library improperly, by attempting to <strong>exit the program</strong>, without having called <code>e4c_context_end()</code> first. The tests begins two exception contexts: the first one is ended properly, but the second one never gets ended because the program exits. The library must signal the misuse by throwing the exception <code>ContextNotEnded</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_EXIT_main",
	"ContextNotEnded"
){

	ECHO(("before_CONTEXT_BEGIN\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_CONTEXT_END\n"));

	e4c_context_end();

	ECHO(("before_CONTEXT_BEGIN_again\n"));

	e4c_context_begin(E4C_TRUE);

	ECHO(("before_EXIT_main\n"));

	return(EXIT_SUCCESS);
}
