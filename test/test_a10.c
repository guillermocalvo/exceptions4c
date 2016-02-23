
# include "testing.h"


DEFINE_TEST(
	a10,
	"e4c_get_exception() without beginning",
	"This test uses the library improperly, by attempting to <strong>call <code>e4c_get_exception()</code></strong>, without having called <code>e4c_context_begin()</code> first. The library must signal the misuse by throwing the exception <code>ContextHasNotBegunYet</code>.",
	NULL,
	EXIT_WHATEVER,
	"before_GET_EXCEPTION",
	"ContextHasNotBegunYet"
){

	ECHO(("before_GET_EXCEPTION"));

	(void)e4c_get_exception();

	ECHO(("after_GET_EXCEPTION\n"));

	return(EXIT_SUCCESS);
}
