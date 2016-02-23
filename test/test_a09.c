
# include "testing.h"


DEFINE_TEST(
	a09,
	"e4c_context_set_handlers() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_context_set_handlers()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_CONTEXT_SET_HANDLERS",
	"ContextHasNotBegunYet"
){

	ECHO(("before_CONTEXT_SET_HANDLERSn"));

	e4c_context_set_handlers(NULL, NULL, NULL, NULL);

	ECHO(("after_CONTEXT_SET_HANDLERS\n"));

	return(EXIT_SUCCESS);
}
